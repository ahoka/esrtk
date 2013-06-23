BUILD_HOST=	$(shell uname -s)

ifeq ($(BUILD_HOST), Darwin)
CROSS=		i686-elf-
COMPILER=	clang
else
CROSS=
COMPILER=	clang
endif

AS=		$(CROSS)as
LD=		$(CROSS)ld
QEMU=		qemu-system-i386

ASFLAGS=	-march=i686 --32

LDFLAGS=	-melf_i386

COPTS=		-O3 -march=i386 -m32 -g3 \
		-Wall -Wextra \
		-nostdlib -fno-builtin \
		-fno-stack-protector

ifeq ($(COMPILER), clang)
#CXXFLAGS+= 
CC=		$(CROSS)clang
CXX=		$(CROSS)clang++
CPP=		$(CROSS)clang -E
COPTS+=		-integrated-as
endif
ifeq ($(COMPILER), gnu)
CXX=		g++
CC=		gcc
CPP=		gcc -E
CXXFLAGS+= 	-fcheck-new -nostartfiles -nodefaultlibs
endif

CFLAGS=		-std=c99 $(COPTS)

CXXFLAGS=	-std=c++11 $(COPTS) \
		-fno-exceptions -fno-rtti \
		-Weffc++

# x86
# TODO: make an Include directory with public API only
IFLAGS+=	-I$(PWD)/Library
IFLAGS+=	-I$(PWD)/Include
IFLAGS+=	-I$(PWD)/Include/X86

CXXFLAGS+=	$(IFLAGS)
CFLAGS+=	$(IFLAGS)

SOURCES_MI:=	Supervisor Library Drivers
SOURCES_X86:=	Drivers Platform

# The blessed extensions are c, hh and S
# CPPFILES:=	$(shell find Supervisor -name '*.cc')
# CPPFILES+=	$(shell find Library -name '*.cc')
# CPPFILES+=	$(shell find Drivers/Mi -name '*.cc')
# CPPFILES+=	$(shell find Drivers/X86 -name '*.cc')

CPPFILES:=	$(shell find $(SOURCES_MI) -name '*.cc')
CPPFILES+=	$(shell find $(SOURCES_X86) -name '*.cc')

CFILES:=	$(shell find $(SOURCES_MI) -name '*.c')

ASMFILES:=	$(shell find $(SOURCES_MI) -name '*.S')
ASMFILES+=	$(shell find $(SOURCES_X86) -name '*.S')

HIDE=	@

all:	debug kernel.img
	@echo Done

.PHONY: debug
debug:
	@echo Build host: $(BUILD_HOST)
	@echo C Compiler: $(CC)
	@echo C++ Compiler: $(CXX)
	@echo C Preprocessor: $(CPP)
	@echo Assembler: $(AS)
	@echo Linker: $(LD)

%.o: %.cc Makefile
	@echo Compiling $<
	$(HIDE) $(CXX) $(CXXFLAGS) -c -o $*.o $*.cc

%.o: %.c Makefile
	@echo Compiling $<
	$(HIDE) $(CC) $(CFLAGS) -c -o $*.o $*.c

%.o: %.S Makefile
	@echo Compiling $<
	$(HIDE) $(CPP) $*.S | $(AS) $(ASFLAGS) -o $*.o

kernel.elf: Loader/MultiLoader.o $(CPPFILES:.cc=.o) $(ASMFILES:.S=.o) $(CFILES:.c=.o)
	@echo Linking kernel executable
	$(HIDE) $(LD) $(LDFLAGS) -T Build/linker.ld -o $@ $^

kernel.img: kernel.elf
	@echo Building kernel image
	$(HIDE) dd if=/dev/zero of=pad bs=1 count=750
	$(HIDE) cat Loader/stage1 Loader/stage2 pad $< > $@

clean:
	@echo Cleaning build files
	$(HIDE) -rm kernel.elf pad kernel.img $(CPPFILES:.cc=.o) $(ASMFILES:.S=.o) $(CFILES:.c=.o) 2>/dev/null

run:
	$(QEMU) -net none -kernel kernel.elf -boot order=c

monitor:
	$(QEMU) -net none -kernel kernel.elf -boot order=c -monitor stdio

run-grub:
	$(QEMU) -fda kernel.img
