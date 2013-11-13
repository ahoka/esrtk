CXX=		clang++
CC=		clang

COPTS+=		-O2 -Wall -Wextra -Werror

CFLAGS+=	-std=c99
CXXFLAGS+= 	-std=c++11

all: $(PROG)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PROG): $(SRC:.cc=.o)
	$(CXX) $(LDFLAGS) $(LIBS) -o $(PROG) $^ $(STATICLIBS)

clean:
	-rm -fr $(PROG) $(SRC:.cc=.o)
