AS=		as
LD=		ld
QEMU=		qemu-system-i386

#COMPILER=	clang
COMPILER=	gnu

ASFLAGS=	-march=i686 --32

LDFLAGS=	-melf_i386

CFLAGS=		-Wall -Wextra -std=c99 \
		-nostdlib -fno-builtin \
		-fno-stack-protector \
		-m32 -march=i686

CXXFLAGS=	-Wall -Wextra -std=c++11 \
		-nostdlib -fno-builtin \
		-fno-exceptions -fno-rtti \
		-fno-stack-protector \
		-m32 -march=i686

ifeq (${COMPILER}, clang)
#CXXFLAGS+= 
CC=		clang
CXX=		clang
else
CXX=		g++
CC=		gcc
CXXFLAGS+= -fcheck-new -nostartfiles -nodefaultlibs
endif


# x86
# TODO: make an Include directory with public API only
CXXFLAGS+=	-I${PWD}
CXXFLAGS+=	-I${PWD}/Supervisor
CXXFLAGS+=	-I${PWD}/Drivers
CXXFLAGS+=	-I${PWD}/Drivers/X86
CXXFLAGS+=	-I${PWD}/Drivers/Console

# The blessed extensions are cpp, h and S
CPPFILES:=	$(shell find . -name '*.cpp')
ASMFILES:=	$(shell find . -name '*.S')

all:	kernel.img
	echo done

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $*.o $*.cpp

%.o: %.S
	${AS} ${ASFLAGS} -o $*.o $*.S

kernel.elf: ${CPPFILES:.cpp=.o} ${ASMFILES:.S=.o}
	${LD} ${LDFLAGS} -T Build/linker.ld -o $@ $^

kernel.img: kernel.elf
	dd if=/dev/zero of=pad bs=1 count=750
	cat Loader/stage1 Loader/stage2 pad $< > $@

clean:
	-rm kernel.elf pad kernel.img ${CPPFILES:.cpp=.o} ${ASMFILES:.S=.o} 2>/dev/null

run:
	${QEMU} -fda kernel.img
