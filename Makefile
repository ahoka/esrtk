AS=		as
LD=		ld
CPP=		cpp
QEMU=		qemu-system-i386

COMPILER?=	clang
#COMPILER?=	gnu

ASFLAGS=	-march=i686 --32

LDFLAGS=	-melf_i386

COPTS=		-O0 -march=i386 -m32 -g3 \
		-Wall -Wextra \
		-nostdlib -fno-builtin \
		-fno-stack-protector

#		-Wlong-long

CFLAGS=		-std=c99 ${COPTS}

CXXFLAGS=	-std=c++11 ${COPTS} \
		-fno-exceptions -fno-rtti \
		-Weffc++

ifeq (${COMPILER}, clang)
#CXXFLAGS+= 
CC=		clang
CXX=		clang++
endif
ifeq (${COMPILER}, gnu)
CXX=		g++
CC=		gcc
CXXFLAGS+= 	-fcheck-new -nostartfiles -nodefaultlibs
endif

# x86
# TODO: make an Include directory with public API only
IFLAGS+=	-I${PWD}/Library
IFLAGS+=	-I${PWD}/Include
IFLAGS+=	-I${PWD}/Include/X86

CXXFLAGS+=	${IFLAGS}
CFLAGS+=	${IFLAGS}

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

all:	kernel.img
	echo done

%.o: %.cc Makefile
	${CXX} ${CXXFLAGS} -c -o $*.o $*.cc

%.o: %.c Makefile
	${CC} ${CFLAGS} -c -o $*.o $*.c

%.o: %.S Makefile
	 ${CPP} $*.S | ${AS} ${ASFLAGS} -o $*.o

kernel.elf: Loader/MultiLoader.o ${CPPFILES:.cc=.o} ${ASMFILES:.S=.o} ${CFILES:.c=.o}
	${LD} ${LDFLAGS} -T Build/linker.ld -o $@ $^

kernel.img: kernel.elf
	dd if=/dev/zero of=pad bs=1 count=750
	cat Loader/stage1 Loader/stage2 pad $< > $@

clean:
	-rm kernel.elf pad kernel.img ${CPPFILES:.cc=.o} ${ASMFILES:.S=.o} ${CFILES:.c=.o} 2>/dev/null

run:
	${QEMU} -net none -kernel kernel.elf -monitor stdio -boot order=c

run-grub:
	${QEMU} -fda kernel.img
