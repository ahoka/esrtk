# Makefile for kernel
#

BUILD_HOST=	$(shell uname)
BUILD_ROOT=	$(PWD)

PLATFORM=	X86

ifeq ($(BUILD_HOST), Darwin)
CROSS=		i686-elf-
else ifeq ($(BUILD_HOST), Msys)
CROSS=		i686-elf-
else
CROSS=
endif

AS=		$(CROSS)as
LD=		$(CROSS)ld
SIZE=		$(CROSS)size
QEMU=		qemu-system-i386
#QEMU=		qemu

LDFLAGS=	-melf_i386

ASFLAGS+=	--32

COPTS=		-O2 -march=i686 -m32 -g3 \
		-Wall -Wextra -Werror \
		-nostdlib -nostdinc -fno-builtin \
		-fno-stack-protector

CFLAGS=		-std=c99 $(COPTS)

CXXFLAGS=	$(COPTS) \
		-fno-exceptions -fno-rtti \
		-Weffc++

CC=		$(CROSS)clang
CXX=		$(CROSS)clang++
CPP=		$(CROSS)clang -m32 -E
COPTS+=		-integrated-as -Weverything
COPTS+=		-Wno-c++98-compat-pedantic \
		-Wno-global-constructors -Wno-exit-time-destructors \
		-Wno-padded -Wno-packed \
		-Wno-weak-vtables
CXXFLAGS+=	-std=c++11

CPPFLAGS+=	-I$(PWD)/Include
CPPFLAGS+=	-I$(PWD)/CInclude
CPPFLAGS+=	-I$(PWD)/CxxInclude
CPPFLAGS+=	-I$(PWD)/Templates

# XXX these should be only provided for Standard
#
CPPFLAGS+=	-I$(PWD)/BsdCompat
CPPFLAGS+=	-DHAVE_NBTOOL_CONFIG_H=0

CPPFLAGS+=	-DHAVE_STRLCAT=0 -DHAVE_STRSEP=0 -DHAVE_STRLCPY=0 -D__ELF__

CFLAGS+=	$(CPPFLAGS)
CXXFLAGS+=	$(CPPFLAGS)

SRCDIR=		Supervisor CLibrary CxxLibrary Drivers Platform FileSystem
TESTDIR=	Test

CCFILES:=	$(shell find $(SRCDIR) -name '*.cc')
CFILES:=	$(shell find $(SRCDIR) -name '*.c')
SFILES:=	$(shell find $(SRCDIR) -name '*.S')

TCCFILES:=	$(shell find $(TESTDIR) -name '*.cc')

SRC=		$(CCFILES) $(CFILES) $(SFILES)

DFILES=		$(CCFILES:.cc=.cc.d) $(SFILES:.S=.S.d) $(CFILES:.c=.c.d)
OFILES=		$(CCFILES:.cc=.o) $(SFILES:.S=.o) $(CFILES:.c=.o)

#HIDE=	@

all:	kernel.elf kernel.img

.PHONY: buildinfo
buildinfo:
	@echo Build host: $(BUILD_HOST)
	@echo C Compiler: $(CC)
	@echo C++ Compiler: $(CXX)
	@echo C Preprocessor: $(CPP)
	@echo Assembler: $(AS)
	@echo Linker: $(LD)

%.o: %.cc
	@echo Compiling $<
	$(HIDE) $(CXX) $(CXXFLAGS) -c -o $*.o $*.cc

%.o: %.c Makefile
	@echo Compiling $<
	$(HIDE) $(CC) $(CFLAGS) -c -o $*.o $*.c

%.o: %.S Makefile
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
	$(HIDE) $(LD) $(LDFLAGS) -T Platform/${PLATFORM}/linker.ld -o $@ $^
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

serial: kernel.elf
	$(QEMU) -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset -nographic

monitor: kernel.elf
	$(QEMU) -net none -kernel kernel.elf -boot order=c -monitor stdio

run-grub: kernel.img
	$(QEMU) -fda kernel.img

-include $(CCFILES:.cc=.cc.d) $(SFILES:.S=.S.d) $(CFILES:.c=.c.d)
