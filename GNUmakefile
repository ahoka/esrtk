# Makefile for kernel
#

BUILD_ROOT := $(dir $(lastword $(MAKEFILE_LIST)))
PLATFORM=	X86

FIND:=		find

TOOLCHAIN?=	gcc
#TOOLCHAIN=	clang

ifeq ($(shell uname -o 2>/dev/null), Msys)
BUILD_HOST:=	Windows
else
BUILD_HOST:=	$(shell uname)
endif

ifeq ($(BUILD_HOST), Windows)
FIND:=		"C:\MinGW\msys\1.0\bin\find.exe"
endif

CROSS=		i686-elf-
COPTS+=		-DCROSS_COMPILER=1

AS=		$(CROSS)as
LD=		$(CROSS)ld
SIZE=		$(CROSS)size

QEMU=		qemu-system-i386

LDFLAGS=	-melf_i386

ASFLAGS+=	--32 -g

COPTS+=		-O2 -march=i686 -m32 -g3 \
		-Wall -Wextra -Werror \
		-nostdlib -nostdinc -fno-builtin \
		-fno-stack-protector

CFLAGS=		-std=c99 $(COPTS)

CXXFLAGS=	$(COPTS) -fno-exceptions -fno-rtti

ifeq ($(TOOLCHAIN), clang)
CC=		$(CROSS)clang
CXX=		$(CROSS)clang++
CPP=		$(CROSS)clang -m32 -E

COPTS+=		-integrated-as -Weverything
COPTS+=		-Wno-c++98-compat-pedantic \
		-Wno-global-constructors -Wno-exit-time-destructors \
		-Wno-padded -Wno-packed \
		-Wno-weak-vtables
else

CC=		$(CROSS)gcc
CXX=		$(CROSS)g++
CPP=		$(CROSS)gcc -m32 -nostdinc -E
GCC_VERSION:=	$(shell $(CC) -dumpversion)
#COPTS+=		-fdiagnostics-color=always
endif

CXXFLAGS+=	-std=c++11

CPPFLAGS+=	-I$(BUILD_ROOT)/Include
CPPFLAGS+=	-I$(BUILD_ROOT)/Include/Kernel
CPPFLAGS+=	-I$(BUILD_ROOT)/CInclude
CPPFLAGS+=	-I$(BUILD_ROOT)/CxxInclude
CPPFLAGS+=	-I$(BUILD_ROOT)/Templates

# XXX unify with the above through transformation
INCDIRS=	$(BUILD_ROOT)/Include $(BUILD_ROOT)/CInclude $(BUILD_ROOT)/CxxInclude $(BUILD_ROOT)/Templates

# XXX these should be only provided for Standard
#
CPPFLAGS+=	-I$(BUILD_ROOT)/BsdCompat
CPPFLAGS+=	-DHAVE_NBTOOL_CONFIG_H=0

CPPFLAGS+=	-DHAVE_STRLCAT=0 -DHAVE_STRSEP=0 -DHAVE_STRLCPY=0 -D__ELF__

CFLAGS+=	$(CPPFLAGS)
CXXFLAGS+=	$(CPPFLAGS)

SRCDIR=		Supervisor CLibrary CxxLibrary Drivers Kernel FileSystem Hal
TESTDIR=	Test

CCFILES:=	$(shell $(FIND) $(SRCDIR) -name '*.cc')
CFILES:=	$(shell $(FIND) $(SRCDIR) -name '*.c')
SFILES:=	$(shell $(FIND) $(SRCDIR) -name '*.S')

TCCFILES:=	$(shell $(FIND) $(TESTDIR) -name '*.cc')

SRC=		$(CCFILES) $(CFILES) $(SFILES)

DFILES=		$(CCFILES:.cc=.cc.d) $(SFILES:.S=.S.d) $(CFILES:.c=.c.d)
OFILES=		$(CCFILES:.cc=.o) $(SFILES:.S=.o) $(CFILES:.c=.o)

#HIDE=	@

all:	Loader/MultiLoader.o kernel.elf kernel.img

.PHONY: buildinfo
buildinfo:
	@echo Build host: $(BUILD_HOST)
	@echo C Compiler: $(CC)
	@echo C++ Compiler: $(CXX)
	@echo C Preprocessor: $(CPP)
	@echo Assembler: $(AS)
	@echo Linker: $(LD)
	@echo Make: $(MAKE)

cppcheck:
	cppcheck -q -i $(INCDIRS) $(SRCDIR)

%.o: %.cc
	@echo Compiling $<
	$(HIDE) $(CXX) $(CXXFLAGS) -c -o $*.o $*.cc

%.o: %.c
	@echo Compiling $<
	$(HIDE) $(CC) $(CFLAGS) -c -o $*.o $*.c

%.o: %.S
	@echo Compiling $<
	$(HIDE) $(CPP) $(CPPFLAGS) -DASSEMBLER $*.S | $(AS) $(ASFLAGS) -o $*.o

%.cc.d: %.cc
	@echo Generating dependencies for $<
	$(HIDE) $(CPP) $(CPPFLAGS) -MM -MT $*.o -MF $<.d $<

%.c.d:  %.c
	@echo Generating dependencies for $<
	$(HIDE) $(CPP) $(CPPFLAGS) -MM -MT $*.o -MF $<.d $<

%.S.d: %.S
	@echo Generating dependencies for $<
	$(HIDE) $(CPP) $(CPPFLAGS) -MM -MT $*.o -MF $<.d $<

depend: $(DFILES)

kernel.elf: Loader/MultiLoader.o $(OFILES)
	@echo Linking kernel executable
	$(HIDE) $(LD) $(LDFLAGS) -T Kernel/${PLATFORM}/linker.ld -o $@ $^
	@$(SIZE) $@

kernel.img: kernel.elf
	@echo Building kernel image
	./mkimage.sh kernel.elf kernel.img

clean:
	@-rm $(DFILES) kernel.elf pad kernel.img $(OFILES) > /dev/null 2>&1 || true

run-isa: kernel.elf
	$(QEMU) -cpu pentium2 -M isapc -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset

run-pc: kernel.elf
	$(QEMU) -M pc -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset

run: kernel.elf
	$(QEMU) -M q35 -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset 2>&1 | tee run.log

run-smp: kernel.elf
	$(QEMU) -M q35 -smp 2 -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset 2>&1 | tee run.log

run-debug: kernel.elf
	$(QEMU) -M q35 -net none -kernel kernel.elf -boot order=c -serial stdio -s -S 2>&1 | tee run.log

serial: kernel.elf
	$(QEMU) -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset -nographic

monitor: kernel.elf
	$(QEMU) -net none -kernel kernel.elf -boot order=c -monitor stdio

run-grub: kernel.img
	$(QEMU) -fda kernel.img

-include $(CCFILES:.cc=.cc.d) $(SFILES:.S=.S.d) $(CFILES:.c=.c.d)
