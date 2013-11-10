CXX?=		clang++
CXXFLAGS=	-std=c++11 -g3 -O0 -fno-inline -Wall -Wextra -Werror \
		-I../ -L.. -Wl,-rpath=../

include ../../gtest/gtest.mk

all: FileTest

FileTest: FileTest.cc
	$(CXX) $(CXXFLAGS) -o FileTest FileTest.cc -lutility_s -lgtest -lpthread

clean:
	-rm FileTest




