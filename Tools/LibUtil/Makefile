include ../tools.mk

CXXFLAGS+=	-fPIC

SRC=		File.cc Directory.cc DirectoryEntry.cc TempFile.cc
OBJ=		$(SRC:.cc=.o)

all: libutility.so libutility_s.a

%.o: %.cc
	@echo Compiling $<
	$(CXX) $(CXXFLAGS) -c -o $*.o $*.cc

libutility.so: $(OBJ)
	@echo Linking dynamic library $<
	$(CXX) -shared $(LDFLAGS) -o libutility.so $(OBJ)

libutility_s.a: $(SRC)
	@echo Linking static library $<
	ar rcs libutility_s.a $(OBJ)

clean:
	-rm libutility.so libutility_s.a $(OBJ)
