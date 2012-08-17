AS=		as
CC=		gcc
CXX=		g++
LD=		ld

CXXFLAGS=	-Wall -Wextra -Werror \
		-nostdlib -fno-builtin -nostartfiles -nodefaultlibs \
		-fno-exceptions -fno-rtti -fno-stack-protector -std=c++0x

CPPFILES=	Drivers/Console/Console.cpp \
		Drivers/Console/Color.cpp \
		Drivers/X86/VgaConsole.cpp \
		Supervisor/Power.cpp \
		Supervisor/Debug.cpp \
		Loader/kmain.cpp


all:	kernel.img

loader.o: Loader/loader.s
	${AS} -o loader.o Loader/loader.s

kmain.o: Loader/kmain.cpp
	${CXX} ${CXXFLAGS} -o kmain.o -c Loader/kmain.cpp

Console.o: Drivers/Console/Console.cpp
	${CXX} ${CXXFLAGS} -o Console.o -c Drivers/Console/Console.cpp

VgaConsole.o: Drivers/X86/VgaConsole.cpp
	${CXX} ${CXXFLAGS} -o VgaConsole.o -c Drivers/X86/VgaConsole.cpp

Power.o: Supervisor/Power.cpp
	${CXX} ${CXXFLAGS} -o Power.o -c Supervisor/Power.cpp

Debug.o: Supervisor/Debug.cpp
	${CXX} ${CXXFLAGS} -o Debug.o -c Supervisor/Debug.cpp

kernel.elf: kmain.o loader.o Console.o VgaConsole.o Power.o Debug.o
	${LD} -T Build/linker.ld -o kernel.elf loader.o kmain.o

kernel.img: kernel.elf
	dd if=/dev/zero of=pad bs=1 count=750
	cat Loader/stage1 Loader/stage2 pad $< > $@

clean:
	-rm *.o *.elf pad *.img

run:
	qemu -fda kernel.img
