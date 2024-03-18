CXX	= g++
CFLAGS	= -O0 -g

all : binarysearch.exe
	./binarysearch.exe

binarysearch.exe : binarysearch.cpp
	$(CXX) $(CFLAGS) -o $@ $<

clean :
	rm -f *.exe *.o *~
