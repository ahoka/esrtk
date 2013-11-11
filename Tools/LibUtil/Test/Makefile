include ../../tools.mk
include ../../gtest/gtest.mk
include ../../LibUtil/libutil.mk

all: FileTest

FileTest.o: FileTest.cc
	$(CXX) $(CXXFLAGS) -c -o FileTest.o FileTest.cc

FileTest: FileTest.o
	$(CXX) $(LDFLAGS) -o FileTest FileTest.o $(STATICLIBS)

clean:
	-rm FileTest *.o




