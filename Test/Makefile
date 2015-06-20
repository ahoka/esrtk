# Test cases for templates

all: testsuite

CPP=g++ -E
CPPFLAGS=-I../Templates

CXX=g++
CXXFLAGS=-std=c++14 -O2 -g $(CPPFLAGS)

SRC=	Catch.cc DoublyLinkedListTest.cc SinglyLinkedListTest.cc
OBJ=	$(SRC:.cc=.cc.o)
DEP=	$(SRC:.cc=.cc.d)

%.cc.d: %.cc
	$(CPP) $(CPPFLAGS) -MM -MT $*.cc.o -MF $<.d $<

%.cc.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $*.cc.o $*.cc

testsuite: $(OBJ)
	$(CXX) $(CXXFLAGS) -o testsuite $^

clean:
	-rm testsuite $(OBJ) $(DEP)

check: testsuite
	./testsuite

-include $(DEP)
