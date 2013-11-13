CXX=		clang++
CC=		clang

COPTS+=		-O2 -Wall -Wextra -Werror

CFLAGS+=	-std=c99
CXXFLAGS+= 	-std=c++11 -fPIC

OBJ=		$(SRC:.cc=.o)

all: $(PROG) $(LIB) $(STATICLIB)

%.o: %.cc
	@echo Compiling object file $<
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PROG): $(OBJ)
	@echo Linking application $<
	$(CXX) $(LDFLAGS) $(LIBS) -o $(PROG) $^ $(STATICLIBS)

$(LIB): $(OBJ)
	@echo Linking dynamic library $<
	$(CXX) -shared $(LDFLAGS) -o $(LIB) $(OBJ)

$(STATICLIB): $(OBJ)
	@echo Linking static library $<
	ar rcs $(STATICLIB) $(OBJ)

clean:
	-rm -fr $(PROG) $(OBJ) $(LIB) $(STATICLIB)
