AS=		as
CC=		gcc
CXX=		g++
LD=		ld
QEMU=		qemu-system-i386

CXXFLAGS=	-Wall -Wextra \
		-nostdlib -fno-builtin -nostartfiles -nodefaultlibs \
		-fno-exceptions -fno-rtti -fno-stack-protector -std=c++0x \
		-I${PWD} -fcheck-new -march=i686


# x86
CXXFLAGS+=	-I${PWD}/Supervisor
CXXFLAGS+=	-I${PWD}/Drivers
CXXFLAGS+=	-I${PWD}/Drivers/X86

CPPFILES:=	$(shell find . -name '*.cpp')

all:	kernel.img
	echo done

%.c:
	${CXX} ${CXXFLAGS} -o $*.o $*.cpp

loader.o: Loader/loader.s
	${AS} -o loader.o Loader/loader.s

kmain.o: Loader/kmain.cpp
	${CXX} ${CXXFLAGS} -o kmain.o -c Loader/kmain.cpp

kernel.elf: ${CPPFILES:.cpp=.o} loader.o
	${LD} -T Build/linker.ld -o $@ $^

kernel.img: kernel.elf
	dd if=/dev/zero of=pad bs=1 count=750
	cat Loader/stage1 Loader/stage2 pad $< > $@

clean:
	-rm loader.o kernel.elf pad kernel.img ${CPPFILES:.cpp=.o}

run:
	${QEMU} -fda kernel.img
