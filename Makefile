AS=		as
LD=		ld
QEMU=		qemu-system-i386

#COMPILER=	clang
COMPILER=	gnu

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
CXXFLAGS+=	-I${PWD}/Library
CXXFLAGS+=	-I${PWD}/Include
CXXFLAGS+=	-I${PWD}/Include/X86

SOURCES_MI:=	Supervisor Library Drivers/Mi
SOURCES_X86:=	Drivers/X86 Platform/X86

# The blessed extensions are c, hh and S
# CPPFILES:=	$(shell find Supervisor -name '*.cc')
# CPPFILES+=	$(shell find Library -name '*.cc')
# CPPFILES+=	$(shell find Drivers/Mi -name '*.cc')
# CPPFILES+=	$(shell find Drivers/X86 -name '*.cc')

CPPFILES:=	$(shell find $(SOURCES_MI) -name '*.cc')
CPPFILES+=	$(shell find $(SOURCES_X86) -name '*.cc')

ASMFILES:=	$(shell find $(SOURCES_MI) -name '*.S')
ASMFILES+=	$(shell find $(SOURCES_X86) -name '*.S')

all:	kernel.img
	echo done

%.o: %.cc Makefile
	${CXX} ${CXXFLAGS} -c -o $*.o $*.cc

%.o: %.S Makefile
	${AS} ${ASFLAGS} -o $*.o $*.S

kernel.elf: Loader/MultiLoader.o ${CPPFILES:.cc=.o} ${ASMFILES:.S=.o}
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
