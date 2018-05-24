#pragma once

#include <cstdint>
#include <algorithm>
#include <tuple>
#include <mpl/mpl.hpp>

using pixel_t = uint32_t;

using namespace kvasir::mpl;

template<typename T, T V> struct value_ { static constexpr T value = V; };

// Lots of endianness assumptions below.
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);

// Convert 8 hex characters to one 32-bit pixel value, byte-order ABGR.
static constexpr uint32_t hex_to_abgr(char c7, char c6, char c5, char c4,
                                      char c3, char c2, char c1, char c0) {

    constexpr auto f = [] (char c) constexpr -> uint32_t {
        return (c >= '0' && c <= '9'
                ? c - '0'
                : c >= 'a' && c <= 'f'
                ? c - 'a' + 10
                : c - 'A' + 10);
    };

    return ((f(c7) << 28) |
            (f(c6) << 24) |
            (f(c5) << 20) |
            (f(c4) << 16) |
            (f(c3) << 12) |
            (f(c2) <<  8) |
            (f(c1) <<  4) |
            (f(c0)));
}

namespace stipje::pixel_format {
    // `format` functions convert (byte-ordered) ABGR to another format
    // (denoted by byte-ordered name).
    // `unformat` convert back to ABGR, this is used to allow the image
    // formatters to be agnostic of the pixel format of the internal buffer.
    // All formats must currently assume pixels are 32-bit wide.

    struct abgr {
        // No-ops.
        static constexpr pixel_t   format(uint32_t x) { return x; }
        static constexpr pixel_t unformat(uint32_t x) { return x; }
    };

    struct rgba {
        static constexpr pixel_t format(uint32_t x) {
            return (((x << 24))            |
                    ((x <<  8) & 0xff0000) |
                    ((x >>  8) & 0x00ff00) |
                    ((x >> 24) & 0x0000ff));
        }
        static constexpr pixel_t unformat(uint32_t x) {
            return (((x << 24))            |
                    ((x <<  8) & 0xff0000) |
                    ((x >>  8) & 0x00ff00) |
                    ((x >> 24) & 0x0000ff));
        }
    };
}

template<typename PixelFormat_>
class ImageProxy {
    // Interface to an image with only the pixel format as a template parameter.

    const pixel_t *pixels_;

    int width_;
    int height_;

public:
    using PixelFormat = PixelFormat_;

    auto width()  const { return width_;  }
    auto height() const { return height_; }
    auto pixels() const { return pixels_; }

    template<typename I>
    ImageProxy(I &img)
        : pixels_(img.pixels),
          width_(img.width()),
          height_(img.height())
    { }
};

template<typename T> ImageProxy(T &img) -> ImageProxy<typename T::PixelFormat>;

namespace stipje::image_format {

    constexpr auto ptorgba = [] (pixel_t p) constexpr {
        return std::make_tuple((p >> 24) & 0xff,
                               (p >> 16) & 0xff,
                               (p >>  8) & 0xff,
                               p & 0xff);
    };

    // PAM format: http://netpbm.sourceforge.net/doc/pam.html
    struct pam {
        template<typename Img, typename S>
        static S &format(S &s, const Img &img) {
            s << "P7\n";
            s << "WIDTH "    << img.width()  << "\n";
            s << "HEIGHT "   << img.height() << "\n";
            s << "DEPTH "    <<            4 << "\n";
            s << "MAXVAL "   <<          255 << "\n";
            s << "TUPLTYPE " << "RGB_ALPHA"  << "\n";
            s << "ENDHDR"                    << "\n";

            if constexpr (std::is_same<typename Img::PixelFormat, pixel_format::rgba>::value) {

                // Pixel format is already RGBA, just dump the pixel buffer.
                s.write((const char*)img.pixels(), img.width() * img.height() * 4);
            } else {
                // Pixel format is different. Convert and write.
                //
                // XXX: Let's hope we have enough stack space.
                //      Don't want to depend on std::array/vector types here,
                //      because we don't want to throw.
                pixel_t buf[img.width() * img.height()];
                for (int i = 0; i < img.width() * img.height(); ++i)
                    buf[i] = pixel_format::rgba::format(Img::PixelFormat::unformat(img.pixels()[i]));
                s.write((const char*)buf, img.width() * img.height() * 4);
            }
            return s;
        }
    };

    // Similar to PAM, but simpler and more widely supported (no alpha channel).
    struct pnm {
        template<typename Img, typename S>
        static S &format(S &s, const Img &img) {
            s << "P6\n" << img.width() << "\n" << img.height() << "\n";
            s << 255 << "\n";
            for (int i = 0; i < img.width() * img.height(); ++i) {
                // We can never write the buffer directly because the PNM
                // format has this weird packed 24-bit pixel format.
                auto p = Img::PixelFormat::unformat(img.pixels()[i]);
                s << (char)(p >> 24);
                s << (char)(p >> 16);
                s << (char)(p >> 8);
            }
            return s;
        }
    };

    // Console output, using 16 color escape codes that should work on anything xterm-like.
    // Each pixel is a single unicode full block character.
    struct console16 {
        template<typename Img, typename S>
        static S &format(S &s, const Img &img) {
            for (int i = 0; i < img.width() * img.height(); ++i) {
                if (i && i % img.width() == 0)
                    s << "\x1b[0m\n";

                auto p = Img::PixelFormat::unformat(img.pixels()[i]);

                auto [r,g,b,a] = ptorgba(p);

                constexpr auto thres = 256/3;

                int c =
                    ((r >= thres)
                     | ((g >= thres) << 1)
                     | ((b >= thres) << 2));

                if (a < 128) {
                    s << "\x1b[0m ";
                } else {

                    s << "\x1b[" << (c + 30);
                    if (std::max({r, g, b}) > thres*2)
                        s << ";1";
                    s << "m█";
                }
            }
            s << "\x1b[0m";
            return s;
        }
    };

    // Console output, using 16 color escape codes that should work on anything xterm-like.
    // This uses unicode half-block characters and background colors to create square pixels.
    struct console16_squared {
        template<typename Img, typename S>
        static S &format(S &s, const Img &img) {
            for (int i = 0;; ++i) {
                if (i && i % img.width() == 0) {
                    s << "\x1b[0m\n";
                    i += img.width(); // Skip even rows.
                    if (i >= img.width() * img.height())
                        break;
                }

                auto tp = Img::PixelFormat::unformat(img.pixels()[i]);
                auto bp = i + img.width() >= img.width() * img.height()
                    ? 0x0000000 // Special case for uneven-height images.
                    : Img::PixelFormat::unformat(img.pixels()[i + img.width()]);

                auto [tr,tg,tb,ta] = ptorgba(tp);
                auto [br,bg,bb,ba] = ptorgba(bp);

                constexpr auto thres = 256/3;

                int tc = ((tr >= thres)
                          | ((tg >= thres) << 1)
                          | ((tb >= thres) << 2));

                int bc = ((br >= thres)
                          | ((bg >= thres) << 1)
                          | ((bb >= thres) << 2));

                if (ta < 128 && ba < 128) {
                    s << "\x1b[0m "; // Transparency case.
                } else if (ta < 128) {
                    s << "\x1b[0m\x1b[" << (bc + 30);
                    s << "m▄"; // Transparent top half case.
                } else if (ba < 128) {
                    s << "\x1b[0m\x1b[" << (tc + 30);
                    s << "m▀"; // Transparent bottom half case.
                } else {
                    s << "\x1b[" << (tc + 30) << ";" << (bc + 40);
                    s << "m▀"; // Full opacity case.
                }
            }
            s << "\x1b[0m";
            return s;
        }
    };

    // Console output, using 24-bit color escape codes. This works on modern terminal emulators.
    // This uses unicode half-block characters and background colors to create square pixels.
    struct console256_squared {
        template<typename Img, typename S>
        static S &format(S &s, const Img &img) {
            for (int i = 0;; ++i) {
                if (i && i % img.width() == 0) {
                    s << "\x1b[0m\n";
                    i += img.width(); // Skip even rows.
                    if (i >= img.width() * img.height())
                        break;
                }

                auto tp = Img::PixelFormat::unformat(img.pixels()[i]);
                auto bp = i + img.width() >= img.width() * img.height()
                    ? 0x0000000 // Special case for uneven-height images.
                    : Img::PixelFormat::unformat(img.pixels()[i + img.width()]);

                auto [tr,tg,tb,ta] = ptorgba(tp);
                auto [br,bg,bb,ba] = ptorgba(bp);

                if (ta < 128 && ba < 128) {
                    s << "\x1b[0m "; // Transparency case.
                } else if (ta < 128) {
                    s << "\x1b[0m\x1b[38;2;" << br << ";" << bg << ";" << bb;
                    s << "m▄"; // Transparent top half case.
                } else if (ba < 128) {
                    s << "\x1b[0m\x1b[38;2;" << tr << ";" << tg << ";" << tb;
                    s << "m▀"; // Transparent bottom half case.
                } else {
                    s << "\x1b[38;2;" << tr << ";" << tg << ";" << tb;
                    s <<     ";48;2;" << br << ";" << bg << ";" << bb;
                    s << "m▀"; // Full opacity case.
                }
            }
            s << "\x1b[0m";
            return s;
        }
    };
}

namespace stipje {
    template<typename S, typename I, typename F>
    void format(S &s, I &img, F) {
        F::format(s, ImageProxy {img});
    }
}

namespace impl {
    template<typename M, typename... Rest>
    struct do_extract_palette;

    template<typename... Ms,
             typename Ch,
             typename ChDiscard, // A space or tab, or whatever.
             typename H7, typename H6, typename H5, typename H4,
             typename H3, typename H2, typename H1, typename H0,
             typename... Rest>
    struct do_extract_palette<list<Ms...>,
                              Ch, ChDiscard,
                              H7, H6, H5, H4,
                              H3, H2, H1, H0,
                              Rest...> {

        // Take a single palette color text (e.g. "# ffddaaff") and turn it into an key/value pair (as a mpl list).

        using type = typename
              do_extract_palette<list<Ms..., list<Ch, uint_<hex_to_abgr(H7::value,
                                                                        H6::value,
                                                                        H5::value,
                                                                        H4::value,
                                                                        H3::value,
                                                                        H2::value,
                                                                        H1::value,
                                                                        H0::value)>>>,
                                 Rest...>::type;
    };

    template<typename... Ms>
    struct do_extract_palette<list<Ms...>> {
        using type = list<Ms...>;
    };
}

template<typename... Rest>
using extract_palette = impl::do_extract_palette<list<>, Rest...>;

template<typename T, T... Cs>
constexpr auto operator ""_stipje_palette() {
    // Return a dummy value to contain the palette type.
    struct X {
        using Palette [[maybe_unused]] = typename extract_palette<value_<T, Cs>...>::type;
    };
    return X { };
}

// Get the value corresponding to a key in a kvlist.
// Kvs... is: list<K,V>...
template<typename K, typename... Kvs>
using assoc_get = call<find_if<unpack<at0<same_as<K>>>,
                               at0<unpack<at1<>>>,
                               always<void>>,
                       Kvs...>;

template<typename T, T... Cs>
constexpr auto operator ""_stipje() {
    // Return a function that will receive the palette, width, height and pixel
    // format, to create an image.

    // C++20 template lambda syntax.
    return [] <typename PT, typename W, typename H, typename Fmt>
              (PT, W, H, Fmt) constexpr {

        // Passing these as lambda parameters purely to get to the type feels dirty.
        // Unfortunately I couldn't explicitly pass template parameters to a lambda...

        static_assert(W::value*H::value == sizeof...(Cs), "incorrect dimensions");

        struct Image {
            // GCC complains when an alias isn't used locally, even though we
            // reference it elsewhere.
            using PixelFormat [[maybe_unused]] = Fmt;

            static constexpr int width () { return W::value; }
            static constexpr int height() { return H::value; }

            pixel_t pixels[sizeof...(Cs)];
        };

        // In one go, transform the list of characters to a pixel buffer, with
        // the given pixel format and palette.
        return Image { Fmt::format(call<unpack<push_front<value_<T, Cs>, cfe<assoc_get>>>,
                                        typename PT::Palette>::value)... };
    };
}
