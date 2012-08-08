AS=		as
CC=		gcc
CXX=		g++
LD=		ld

CXXFLAGS=	-Wall -Wextra -Werror \
		-nostdlib -fno-builtin -nostartfiles -nodefaultlibs \
		-fno-exceptions -fno-rtti -fno-stack-protector -std=c++0x

all:	kernel.img

loader.o: Loader/loader.s
	${AS} -o loader.o Loader/loader.s

kmain.o: Loader/kmain.cpp
	${CXX} ${CXXFLAGS} -o kmain.o -c Loader/kmain.cpp

Console.o: Drivers/Console/Console.cpp
	${CXX} ${CXXFLAGS} -o Console.o -c Drivers/Console/Console.cpp

kernel.elf: kmain.o loader.o Console.o
	${LD} -T Build/linker.ld -o kernel.elf loader.o kmain.o

kernel.img: kernel.elf
	dd if=/dev/zero of=pad bs=1 count=750
	cat Loader/stage1 Loader/stage2 pad $< > $@

clean:
	-rm *.o *.elf pad *.img
