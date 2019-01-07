example: example.cc stipje.hh
	$(CXX) -I. -Wall -Wextra -std=c++2a -Wno-trigraphs -O1 -o $@ $<

.PHONY: clean

clean:
	rm -f example
