example: example.cc stipje.hh
	$(CXX) -Impl/src/kvasir -Wall -Wextra -std=c++2a -Wno-trigraphs -Os -o $@ $<

.PHONY: clean

clean:
	rm -f example
