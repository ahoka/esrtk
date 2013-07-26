BUILD_HOST=	$(shell uname -s)

COMPILER=clang

ifeq ($(BUILD_HOST), Darwin)
CROSS=		i686-elf-
COMPILER=	clang
ASFLAGS=	
else
CROSS=
COMPILER?=	clang
#ASFLAGS=	-O3 -march=i686 -m32
endif

AS=		$(CROSS)as
LD=		$(CROSS)ld
QEMU=		qemu-system-i386

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

ifeq ($(COMPILER), clang)
CC=		$(CROSS)clang
CXX=		$(CROSS)clang++
CPP=		$(CROSS)clang -m32 -E
COPTS+=		-integrated-as -Weverything
COPTS+=		-Wno-c++98-compat-pedantic \
		-Wno-global-constructors -Wno-exit-time-destructors \
		-Wno-padded -Wno-packed
CXXFLAGS+=	-std=c++11
endif
ifeq ($(COMPILER), gnu)
CXX=		g++
CC=		gcc
CPP=		gcc -m32 -E
CXXFLAGS+= 	-fcheck-new -nostartfiles -nodefaultlibs -std=c++0x
endif

# x86
# TODO: make an Include directory with public API only
CPPFLAGS+=	-I$(PWD)/Include
CPPFLAGS+=	-I$(PWD)/Standard

CFLAGS+=	$(CPPFLAGS)
CXXFLAGS+=	$(CPPFLAGS)
#ASFLAGS+=	$(CPPFLAGS)

SRCDIR=		Supervisor Library Drivers Platform

CCFILES:=	$(shell find $(SRCDIR) -name '*.cc')
CFILES:=	$(shell find $(SRCDIR) -name '*.c')
SFILES:=	$(shell find $(SRCDIR) -name '*.S')

SRC=		$(CCFILES) $(CFILES) $(SFILES)

DFILES=		$(CCFILES:.cc=.cc.d) $(SFILES:.S=.S.d) $(CFILES:.c=.c.d)
OFILES=		$(CCFILES:.cc=.o) $(SFILES:.S=.o) $(CFILES:.c=.o)

#HIDE=	@

all:	kernel.img

.PHONY: buildinfo
buildinfo:
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

#ifeq ($(BUILD_HOST), Darwin)
%.o: %.S Makefile
	@echo Compiling $<
	$(HIDE) $(CPP) $(CPPFLAGS) -DASSEMBLER $*.S | $(AS) $(ASFLAGS) -o $*.o
#else
#%.o: %.S Makefile
#	@echo Compiling $<
#	$(HIDE) $(CC) $(ASFLAGS) -c -o $*.o $*.S
#endif

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
	$(HIDE) $(LD) $(LDFLAGS) -T Build/linker.ld -o $@ $^

kernel.img: kernel.elf
	@echo Building kernel image
	$(HIDE) dd if=/dev/zero of=pad bs=1 count=750
	$(HIDE) cat Loader/stage1 Loader/stage2 pad $< > $@

clean:
	$(HIDE) -rm $(DFILES) kernel.elf pad kernel.img $(OFILES) 2>/dev/null

run: kernel.elf
	$(QEMU) -net none -kernel kernel.elf -boot order=c -serial stdio

monitor:
	$(QEMU) -net none -kernel kernel.elf -boot order=c -monitor stdio

run-grub:
	$(QEMU) -fda kernel.img

-include $(CCFILES:.cc=.cc.d) $(SFILES:.S=.S.d) $(CFILES:.c=.c.d)
