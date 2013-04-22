AS=		as
LD=		ld
QEMU=		qemu-system-i386

#COMPILER=	clang
COMPILER=	gnu

ASFLAGS=	-march=i686 --32

LDFLAGS=	-melf_i386

COPTS=		-O2 -march=i386 -m32 \
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
CXXFLAGS+=	-I${PWD}/Library
CXXFLAGS+=	-I${PWD}/Include
CXXFLAGS+=	-I${PWD}/Include/X86

# The blessed extensions are c, hh and S
CPPFILES:=	$(shell find . -name '*.cc')
ASMFILES:=	$(shell find . -name '*.S')

all:	kernel.img
	echo done

%.o: %.cc Makefile
	${CXX} ${CXXFLAGS} -c -o $*.o $*.cc

%.o: %.S Makefile
	${AS} ${ASFLAGS} -o $*.o $*.S

kernel.elf: Loader/GrubLoader.o ${CPPFILES:.cc=.o} ${ASMFILES:.S=.o}
	${LD} ${LDFLAGS} -T Build/linker.ld -o $@ $^

kernel.img: kernel.elf
	dd if=/dev/zero of=pad bs=1 count=750
	cat Loader/stage1 Loader/stage2 pad $< > $@

clean:
	-rm kernel.elf pad kernel.img ${CPPFILES:.cc=.o} ${ASMFILES:.S=.o} 2>/dev/null

run:
	${QEMU} -kernel kernel.elf

run-grub:
	${QEMU} -fda kernel.img
