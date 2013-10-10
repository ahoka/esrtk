# Test cases for templates

all: LinkedListTest

CPP=clang++ -E
CPPFLAGS=-std=c++11 -I../Templates

CXX=clang++
CXXFLAGS=-O0 -g3 $(CPPFLAGS)

%.cc.d: %.cc
	$(CPP) $(CPPFLAGS) -MM -MT $*.o -MF $<.d $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $*.o $*.cc

LinkedListTest: LinkedListTest.o
	$(CXX) $(CXXFLAGS) -o LinkedListTest $^

clean:
	-rm LinkedListTest

-include LinkedListTest.cc.d
