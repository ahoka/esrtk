# Test cases for templates

all: SinglyLinkedListTest DoublyLinkedListTest

CPP=clang++ -E
CPPFLAGS=-std=c++11 -I../Templates

CXX=clang++
CXXFLAGS=-O2 -g3 $(CPPFLAGS)

%.cc.d: %.cc
	$(CPP) $(CPPFLAGS) -MM -MT $*.o -MF $<.d $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $*.o $*.cc

DoublyLinkedListTest: DoublyLinkedListTest.o
	$(CXX) $(CXXFLAGS) -o DoublyLinkedListTest $^

SinglyLinkedListTest: SinglyLinkedListTest.o
	$(CXX) $(CXXFLAGS) -o SinglyLinkedListTest $^

clean:
	-rm DoublyLinkedListTest SinglyLinkedListTest *.o

-include DoublyLinkedListTest.cc.d SinglyLinkedListTest.cc.d
