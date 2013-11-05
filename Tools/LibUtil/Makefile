CXX=		clang++
CXXFLAGS=	-std=c++11 -O2 -Wall -Wextra -Werror

SRC=		File.cc Directory.cc DirectoryEntry.cc

all: libutility.so

libutility.so: $(SRC)
	$(CXX) -shared -fPIC $(CXXFLAGS) -o libutility.so $(SRC)

clean:
	-rm libutility.so
