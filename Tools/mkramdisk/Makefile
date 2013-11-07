CXX=		clang++
CXXFLAGS=	-std=c++11 -O2 -Wall -Wextra -Werror -I../LibUtil -L../LibUtil

all: mkramdisk

mkramdisk: mkramdisk.cc
	$(CXX) $(CXXFLAGS) -o mkramdisk mkramdisk.cc

clean:
	-rm mkramdisk


