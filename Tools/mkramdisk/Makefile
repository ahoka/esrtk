CXX?=		clang++
CXXFLAGS=	-std=c++11 -g3 -O0 -fno-inline -Wall -Wextra -Werror -I../LibUtil -L../LibUtil -Wl,-rpath=../LibUtil

all: mkramdisk

mkramdisk: mkramdisk.cc
	$(CXX) $(CXXFLAGS) -o mkramdisk mkramdisk.cc -lutility_s

clean:
	-rm mkramdisk


