#include "stipje.hh"
#include <iostream>

int main() {

    {
        constexpr auto glider =
        "                                                 "
        " ooooooo                                         "
        " o     o   ####  ##### # #####       # ######    "
        " o  #  o  #        #   # #    #      # #         "
        " o   # o   ####    #   # #    #      # #####     "
        " o ### o       #   #   # #####       # #         "
        " o     o  #    #   #   # #      #    # #         "
        " ooooooo   ####    #   # #       ####  ######  o "
        "                                                 "_stipje
        ("  00000000"
         "o ff0000ff"
         "# ffffffff"_stipje_palette,
         uint_<49> {},
         uint_< 9> {},
         stipje::pixel_format::rgba{});
        stipje::format(std::cout, glider, stipje::image_format::console256_squared{});
    }

    {
        constexpr auto lena =
"NNNNNNNNFNDDDrrrrlrriiaa}:~--`----`'```,,,   -yQQQNvkkkkksFT3%3XNFvvv"
"KQNNNNNNNrFrrqrsqrrrlii(:~~~~+-~~~``,`,````,` -IMNNEkkkktEKT7%$ZTFrkv"
"QKQNKNFDFDsrrsqlsorrrh>>:=:~=~=-`'````,` ``  ` >SNNstkkkkrKT7$$7ZNDss"
"QNQKMKMFDFrrsqrorrDq}(::::=:=~-~-``` ,`,` ,`,   gMDEkkkksvNT29$$3SB>:"
"QNNQNFNFDrrrsrrqDzq[>:>>:*>:*~~-~`-`-,-,-, ,    `MDskkkkksFT2$$$3b`,+"
"KQKNNNFFFsDDrsDDDh[>[>(>>>*::~-~-~--'```,`,,`,,  gNEkkkklvKT27$W;`=~ "
"QNNNKNFrFDDFrFFCa[[>[[:(>>*=**^++=:~-`'---,,,,,, ,CskkkkkvFTZZx~`; ``"
"KNQNNFDFNFDDrDCa)[))((>>>>:*::=**----=~-``,',',,, /rkkkkkrETZb`~; ```"
"QQNMFFNFFrrDDri)aa[[(>[>>:::>>>::+-~----^^~+,''',,,^kkkkfvrp~`:~``,--"
"QQNNNNFDDFFFrhala}})>[:>(>>>(>>=*:=+=---;:+--',''',,*kkkkh:`~=` ``--'"
"NQQNMNFFNFMslfcaiaa[>([(>>(>>>>>>>>*=-+>!=+--++++^''+kk[>~~:~``-----'"
"KQQNNFFNFDrljlhiaa))((((()(((>>>>!*=!(>!!**+++^++;+^'<[~~~:``--`'-'--"
"KQKKQNFFFrlillqjia}]}(>(a[(>[>((>>;>>>==;=;+!;/^b^b^+~~~:``-`---'`++-"
"NQQMNFFFrrlrirlhahha((a}((([>)>((>>!>!>!+!!/!b^/^//+~::~`,--------+(+"
"KNQNNCNrrslqqllahja}}}a(([(((((}()((>/!/;!/!!///b!~::~`~~-+----`-=}}!"
"EKQNNNDrrrqCrjiiiahiaa)[}})}[)}[{a{(((>!/////<{!:::-~`+~~+-------{}{h"
"FNQKNrrrrrDrlliiihai?a?a(}]{a}}}(}}{({{/{{/{b/*:>=~-~=++~~-`+`,-;{(he"
"vENNFrrsCorllrrrhiihaeheemge{}ea}})}}){]{{{}>:[:~~=;=+++--`---,-{aheh"
"kENFrsrrDrrrrqCqiqieinnmeygww}e}eee{ee{dbd/>>>=~+==+~~---,--,-'>ghCCe"
"kvEFFDDDslssCqrqqihCJnISOOedwGwwggmmmxPme(>(:~~:====-+--'-,--,;gBTMp="
"kfFFFCDDsrrDFqozqqBpBJR0VVxmuPHUYYV1451w(a(:~=+=++++----`'-';{BMMMq/;"
"kFDDCNDrrrCrCoCqoCBCRJIOVVUHY44YY4@##5m[h:~+=*=+~=~++++'-+;gJMMNCh[>x"
"frsFCDrsCDDqCCsqBJCMOVV5961UUYU144#@5ghg:>:^==;=;+=+++++;mMNQMCqa[/7#"
"kfFNFrsCCDCCDsrDSDJW61&@&YYY&1Y44Y@1gh(::>;=;;:;;=++++;bxwCFrfff[/W$$"
"kiFDrsDNCszBICDrRS665@@@@1V&4Y44#YP[h>:>/>!!!;;;;;+;;mOPGphf[f[[(1#%%"
"frDFsCCCDCCDCISI0655@@##55%51Y4Y#uhp>::>;/;///!;;;;bOPxGGmaf[f[g1%%@@"
"frrsDNDzCMDDMRW3664@#4#@4##5544HVgp::;;;;///;/;;;/}JJOPxGxef[[g###%&&"
"fvsDDzCCBMIMMTZ66@##45##%#%@@4UPxw>::=;//;//;;!!!;{BSOOOxwn[aJ##%%@%%"
"ffDDCzzNCCMRW77W6@#@&@@###5554UHub;>;////////;!!!^!{IWGVOxyBZ###@&@@&"
"fsDNDMNDDNTS36@@5%@@#4@#####5RWub[/>;/{;/;//!!^!!!!}CT7WWGwBZ##@@%%%%"
"vrDCNzDFMM366##@5&@#@4Y####%JJgb/;;>/{///{(/^!^!^^!)nT38WGwJ7@#@@&@@@"
"FFNNNDMMTX$6@####%@@@#Y4@#@qqg}bb/;;/{/>{)/!!^^^^^^!iS3&3PmO%#&%%%%%@"
"FDNDFDTZ33%6##@##@#@4@Y##@pqg[(b/;;///{a})<!^!^^^''^]I2@91xw9%@@@@@%@"
"EFDDDNT2%#@4#@#####@Y4YU#Jr[[>//b;:/}}e])<!!^^^^^^'^{CX%%6wy%#%&%&%@@"
"EEFDNT2@5##@@####@#@@@#PJrh((:;b{;/{ga})}<!!^^^^''^'<oT9%6Ow7@%@%@@%@"
"AABJR35#4@##@5@##@@#54#VCpaa};;;//}e}}])))<!!*^^^'^^!nT7@6Pw3%@%%%%@%"
"vIINS&4#4##@4@###@4@5#Wqh[[[{;;;/mpdd}]<)<)<**^^^^^^^dT7@%VmW9%@%%@@%"
"BaNT61@@Y#@#####@@@45$pJ[[>{};;{JIJRwwe{())<!<!<!*^/eJ3%@@3mR$%%@@%%$"
"CCX254#4Y####@###445@Br[[:>g>;gpqqiISXRyd]})>!<!<{eORSX7%@6wO%%%%@@&$"
"JT3%Y&444@#@#4####5#WC[[>=(/;pJMJ0yJRIQQBic]<>!<dySRR033&@7mR75@@%@$%"
"7@5YY44##4####4@@#@6hg[[:;//OZ37%@&%8RIDDoj]<!/{yW8%@%$$#@6wJ6%6@&@%6"
"@15Y@@U###@4##44@%#Jp([>;;/O2$%#@6xx13SNLsjc(!<O%@1O$@@#@@7wGW9@@%$%%"
"Y@5#4Y4####4####@@WC[g>;;>JX36316Wb;{XTMLDoc!^mZ661wZ##@@&6yw16&@@%%3"
"#@@&4YU####@######Jp{[:=;wIJSXXRVG;!}zLMMzo]^^ByW1GpZ7&@@%7wwW6%@%5%W"
"@444YYU@#########RB>[;=;BCCBBySByg/{bdzLNDt]^^nyynpJ277%@$7Om31%@%$6R"
"#44&YUUY@#######$Cp[>;;BMCqoqqqBIyynddnzzAod'^bnzSI00079%66OmW7&@$53C"
"#@&44YUY@####@##Jr[[;;JNzzoddoojddddddooottd^'<ddIISSX39977OeV7%@$$WC"
"##44UYYU#######Wrp[;;wMMzoodd]]d]]]]]jddoood^'/{doCMMS2$$78ReO6%%$$Rp"
"##4Y4UHVW%#####JC[>;mSQNMzojd])])]{]]jooootd^'^bdoAIIJ07736OeO6%%67Cr"
"###%YY#ZR3####ZCg(;b3TTMzzojd)])]<]]dcdjozzd^'!]ddzzII28737Wmw6%6$ZCq"
"#####6@SJR####CC[>;$ZXQLzAojj]]))]]]ccjooAzo<'^{dnzzII2$737Wew6%66Rqh"
"####%$$ZRqW##Wrg;;P72XLMLztojc]]])]]djjoozzdd'^]dooBzS79$26WnyW577Jql"
"###623ZJBZq#$CC>;b#32XQLzztojcc]]]]ccjoAzzzo<^'ddqzIyS%%7373mwW$6ZChl"
"##@7SZRpa3Z#7Cp;/@%32XTMLLAoojdc]]ccjoozzLIod^'{dnzzI0@%7293nnO1$Zqra"
"##@13CNZW%%7JCgg1#%30XQLMzAtojjjcccdjozLAzLtd^,]joIII7#&3267wpJ71Jlla"
"#%$1pZBW%%#ghhgp1%%3XXTQLzAtojjjjccjozLAAoAAd<'ejBzI0@@$7377wpp17Cfha"
"@%##$1O$#%Og>(g1#@&30XTQLLzttojjjjjjozzLTQLLt]<eoBII7#@93377Oey3Zhlfa"
"$ZW##V1671Jhg{1%@%%30XTMNzAAoojjjcccjozLLALLn{ddqzIR@#@%3377RmpWJlfaa"
"6RC$$WWRJgJC;P#%%%&3XXTLQLztttojjjjjijooojid!/ddyzT6@#%92337WeyRClhaa"
"7ZZ1$1Bp{pSgb%%@&$$30XTTLDLAoottjjjcjjjjj]!<!!dozJR%##%733373mpRqfaaa"
"WWW$Zpg{/SJg$%%$9$6300TQLLzLAoooojjjjojjd<^{^/eqMM$###9323373ynJflfha"
"GZ7JJpg>JJg%99@%66%10XXQLMLAzAAtooooooojd<^/^<dIIR###@9333377ypJhfaaa"
"H1Jp/;;pTgV#77@%77930XXTQLMAAzztALNzzCzzzzodyyyQI6#@#%7723337ypqlfhha"
"UPnp;;pCpm%%%8%$67%830XQLMLLzAzAzTTXTQQTXTLITSTIR##@#%7332337Onhfaaah"
"YOkv+/CJg#6@@9@%$798300TQLMMzzzDzDLLLLLdddbddIIJ6#@@@%73227Z3Rplfaaaa"
"Uwtr;Jfg1#@@#@%%6889330XQQLLMNzzsANLTLLdddddIyMW###@@9$230333Realahah"
"Pmnn/f[W#@%@#@%&$669332XQQMLLMzzzDzzLILLzyIyyIR9##%%%660302Z3Relaah{a"
"OOOg[[p###@%@@#%%$7%6830SQLMMLNzozzAMzzIzzooyI3@##@%$77320203Ralhagah"
"OOG/:q###@@%@@@@&%$$$8930TQQMMNDzzooooddddnzyR8###%%%670303Z3W{ahaa[h"
"OIWpg$###@%@@%%#%%9%$$8782XTTLMzozoodddccddBy87##%%9993Z00200Relhhghh"
"GyWWP$###&9%@@@##%%9%$789%8XXQLMzBoqj]cd]ddBI%6##@69$$30Z002ZW}hhh[ga"
"HxOW7####%7@%%@@##%%%%$98%%82XSQMMzood]dddqyS69@#%9$663X02ZX00ehhphhh"
"Huuxx####%7@@%@@@@@%9$$$37732XSSIIIyoddddnnnS7%@#%69993Z00WXZRhhCJCpq"
"UuHVxu4#@83@9%@@@@@&%%$7732TSIIBMMzqiqiihd}(!{b$%%77$7WX02X00OhrCCMMC"
"UUHUWxu&%72@%%%#%@@@%9$9800TNMMMMIzqooCoi])(!!!;gV$6$7ZX002XXRiqCCCCM"
"YUUUumVu973@9%@#@@@@&%9973XTSNMLCCCozqoqj})>!:*!+;/P$30RZZXX0OlqppqqC"
"UUHUPu1PV33@8$@#@@@@%$8773XTTMMMMzzrzooii))>>>*!:=;=m7ZX02ZXZBlplhqpq"
"UUHuHWu6G37@99%@#%%%%98383XTQNLLDsrzsDlj??<[<>>>**=+-bZX02X0ZJlhphhhq"
"YUHHuUGuPG8@9$%@@@%@%72273XTQIMzzzssssic??)()>>>*:*+--/ZZ0ZXZqhhhhphh"
"YYHHHHPxPx9@99%@@%%%%73X230STQAADsssosjic]})))<<>*=^-''b0Z2X2prhhhhph"
"4YUPHuGWbb6@999@#%%%%32X23XTMDzAzsDssjki?c?))[><>**^+''-wZX2Zrhhhhhgh"
"Y4UHHuu%VbG@999%@%9@97XTX7XTNzDAAssstsjjc???)<)><**--'',;ZXZZfphhhhhh"
"YY4UuuP1VbP#778%@%9&82XX07XQNLDAAtsrsksjc???)))>>**^'''''wZ2Sfrhhhhhp"
"YYYYuHu81b1@387%%%%%82XTX3XLNFAFtDttsojjic???))<<>:^''',,;ZZMhhhhhhhh"
"YU4UuuHGuuu@279%9%$92XQQX2XMLFAAtsssstsjlj???))[>*^^'''''-W3Cfhghhhhh"
"4UYHuuuuuVY63879$9$22TTQX2TLFAAsAAAtsltjjic????<<!*^^''',,b7Cfhhghphh"
"&&UHHPVuxxGV07377872XQQTX0TQNFAsttAttsskjjcc?)))<!^^'''''';Zrhh[hhhph"
"@&VHUHU9113WVw077872XQLQ0XQDDFAtAAtAsttskjc??c))(<!+^'''''-Zfhghghhhh"_stipje
("# 080201ff" "@ 130506ff" "& 190509ff" "% 1b0904ff" "$ 200f03ff" "9 230d05ff" "8 270d06ff" "7 2a1305ff"
 "6 26130dff" "5 211416ff" "4 1f151dff" "3 34190dff" "2 3a1a08ff" "1 311f1cff" "Z 402312ff" "0 432012ff"
 "Y 2c202bff" "X 4a2310ff" "W 3f251aff" "V 3f2a28ff" "U 392937ff" "T 5c2f18ff" "S 59311dff" "R 543224ff"
 "Q 653318ff" "P 4e3636ff" "O 583a31ff" "N 6f3b1dff" "M 6a3b24ff" "K 753e18ff" "L 733820ff" "J 633e2cff"
 "H 503b48ff" "I 6b3d2bff" "G 5d4241ff" "F 7a4523ff" "E 804820ff" "D 7b472aff" "C 76492fff" "B 744934ff"
 "A 82452aff" "z 7d4530ff" "y 724a3cff" "w 6e4d44ff" "x 684c4bff" "v 8b522aff" "u 634d59ff" "t 8c4d32ff"
 "s 895132ff" "r 865434ff" "q 83563bff" "p 7b5946ff" "o 8c523dff" "n 825645ff" "m 795a52ff" "l 926141ff"
 "k 9e6139ff" "i 936149ff" "j 985d48ff" "h 8e664cff" "g 866756ff" "f 9b6841ff" "e 906557ff" "d 986357ff"
 "c a16753ff" "b 866e6cff" "a 9c7156ff" "? a36f58ff" "} 9d7461ff" "] a47062ff" "{ 997568ff" "[ a98164ff"
 ") ac7e6dff" "( a8836fff" "/ a4857bff" "< b9867bff" "> b1917dff" "! b29288ff" "; ad968aff" ": b99f8aff"
 "* bc9c90ff" "= baa394ff" "+ b9a59cff" "^ bfa19cff" "~ c3af9fff" "- c4b2a8ff" "' c6b4b1ff" "` cdbfb4ff"
 ", cdc1bdff" "  d6d0ccff"_stipje_palette,
 uint_<69> {},
 uint_<88> {},
 stipje::pixel_format::rgba{});

        stipje::format(std::cout, lena, stipje::image_format::console256_squared{});
    }

    return 0;
}
