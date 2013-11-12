CXX?=		clang++
CXXFLAGS=	-std=c++11 -g3 -O0 -fno-inline -Wall -Wextra -Werror -I../LibUtil -L../LibUtil -Wl,-rpath=../LibUtil

all: mkfatfs

mkfatfs: mkfatfs.cc
	$(CXX) $(CXXFLAGS) -o mkfatfs mkfatfs.cc -lutility_s

clean:
	-rm mkfatfs


