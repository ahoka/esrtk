# Makefile for ESRTK
#

.ifndef HAS_BUILD_SH
.error "Please run the build using build.sh"
.endif

#BUILD_ROOT := $(dir $(lastword $(MAKEFILE_LIST}}}
BUILD_ROOT := ${.CURDIR}
PLATFORM=	X86

FIND:=		find

TOOLCHAIN?=	gcc
#TOOLCHAIN=	clang

BUILD_HOST!=	uname

CROSS=		i686-elf-

AS=		${CC}
LD=		${CROSS}g++
SIZE=		${CROSS}size

QEMU=		qemu-system-i386

QEMU_ARGS=	-m 64M -M q35 -watchdog i6300esb -device rtl8139 -boot order=c -serial stdio -d cpu_reset -no-reboot

# XXX -flto
COPTS+=		-O2 -march=i686 -g \
		-Wall -Wextra \
		-nostdlib -nostdinc -fno-builtin \
		-fno-omit-frame-pointer \
		-fno-stack-protector

# XXX ACPICA
#COPTS+=		-Werror

LDFLAGS=	${COPTS}
ASFLAGS+=	-g ${COPTS}
CFLAGS=		-std=c11 ${COPTS}
CXXFLAGS=	-std=c++14 ${COPTS} -fno-exceptions -fno-rtti

.if ${TOOLCHAIN} == clang
CC=		${CROSS}clang
CXX=		${CROSS}clang++
CPP=		${CROSS}clang -m32 -E

COPTS+=		-integrated-as -Weverything
COPTS+=		-Wno-c++98-compat-pedantic \
		-Wno-global-constructors -Wno-exit-time-destructors \
		-Wno-padded -Wno-packed \
		-Wno-weak-vtables
.else
CC=		${CROSS}gcc
CXX=		${CROSS}g++
CPP=		${CROSS}cpp -m32 -nostdinc
GCC_VERSION!=	${CC} -dumpversion

.if ${GCC_VERSION:C/^4.9//} != ${GCC_VERSION}
COPTS+=		-fdiagnostics-color=always
.endif

.endif

INCDIRS=	Include CInclude CxxInclude Templates Include/Kernel

INCPATHS=	${INCDIRS:S/^/${BUILD_ROOT}\//}

# ACPI headers are included as system headers to supress 3pp warnings
#
CPPFLAGS+=	${INCPATHS:S/^/-I/} -isystem${BUILD_ROOT}/Acpi/Include

# XXX these should be only provided for Standard
#
CPPFLAGS+=	-I${BUILD_ROOT}/BsdCompat
CPPFLAGS+=	-DHAVE_NBTOOL_CONFIG_H=0 -DLIBCXXABI_BAREMETAL=1

CPPFLAGS+=	-DHAVE_STRLCAT=0 -DHAVE_STRSEP=0 -DHAVE_STRLCPY=0 -D__ELF__

CPPFLAGS+=	-D__esrtk__
#CPPFLAGS+=	-D_BSD_SOURCE

SRCDIR=		Supervisor CLibrary CxxLibrary CxxAbi Drivers Kernel FileSystem Hal Loader Acpi/Source
TESTDIR=	Test

TCCFILES:=	${FIND} ${TESTDIR} -name '*.cc'

SRC!=		(cd ${BUILD_ROOT} && find ${SRCDIR} \( -name '*.c' -or -name '*.cc' -or -name '*.cpp' -or -name '*.S' \) -exec basename {} \;)
DIR!=		(cd ${BUILD_ROOT} && find ${SRCDIR} \( -name '*.c' -or -name '*.cc' -or -name '*.cpp' -or -name '*.S' \) -exec dirname {} \; | sort -u)

OFILES=		${SRC:C/\.(cc|cpp|c|S)$/.o/}
DFILES=		${SRC:C/\.(cc|cpp|c|S)$/.d/}

.PATH:		${DIR:S/^/${BUILD_ROOT}\//}

#HIDE=	@
.SUFFIXES:	.c .cc .cpp .S .d .o

all:	MultiLoader.o kernel.elf

MultiLoader.o: MultiLoader.S

.PHONY: buildinfo
buildinfo:
	@echo Build host: ${BUILD_HOST}
	@echo C Compiler: ${CC}
	@echo C++ Compiler: ${CXX}
	@echo C Preprocessor: ${CPP}
	@echo Assembler: ${AS}
	@echo Linker: ${LD}
	@echo Make: ${MAKE}
	@echo Objdir: ${.OBJDIR}
	@echo Src: ${SRC}
	@echo Dir: ${DIR}
	@echo o-files: ${OFILES}
	@echo d-files: ${DFILES}

cppcheck:
	cppcheck --inline-suppr --template=gcc -q -i ${INCDIRS} ${SRCDIR:C/^/${BUILD_ROOT}\//}

.cc.o .cpp.o:
	@echo Compiling $<
	${HIDE} ${CXX} ${CXXFLAGS} ${CPPFLAGS} -c -o ${.TARGET} ${.IMPSRC}

.cpp.o:
	@echo Compiling $<
	${HIDE} ${CXX} ${CXXFLAGS} ${CPPFLAGS} -c -o ${.TARGET} ${.IMPSRC}

.c.o:
	@echo Compiling $<
	${HIDE} ${CC} ${CFLAGS} ${CPPFLAGS} -c -o ${.TARGET} ${.IMPSRC}

.S.o:
	@echo Compiling $<
	${HIDE} ${CC} ${ASFLAGS} ${CPPFLAGS} -c -traditional-cpp -o ${.TARGET} ${.IMPSRC}

.cpp.d:
	${HIDE} ${CPP} ${CPPFLAGS} -MM -MT $*.o -MF ${.TARGET} ${.IMPSRC}

.cc.d:
	${HIDE} ${CPP} ${CPPFLAGS} -MM -MT $*.o -MF ${.TARGET} ${.IMPSRC}

.c.d:
	${HIDE} ${CPP} ${CPPFLAGS} -MM -MT $*.o -MF ${.TARGET} ${.IMPSRC}

.S.d:
	${HIDE} ${CPP} ${CPPFLAGS} -MM -MT $*.o -MF ${.TARGET} ${.IMPSRC}

depend: .depend

.NOPATH: .depend
.depend: ${DFILES}
	@cat ${DFILES} > ${BUILD_ROOT}/.depend

.PHONY: autobuild
autobuild: clean .WAIT kernel.elf
	$(CROSS)strip kernel.elf

kernel.elf: MultiLoader.o ${OFILES}
	@echo Linking kernel executable
	${HIDE} ${LD} ${LDFLAGS} -T ${BUILD_ROOT}/Kernel/${PLATFORM}/linker.ld -o ${.TARGET} ${.ALLSRC}
	@${SIZE} $@

kernel.img: kernel.elf
	@echo Building kernel image
	env BUILD_ROOT=${BUILD_ROOT:Q}/ ${BUILD_ROOT}/mkimage.sh kernel.elf kernel.img

clean:
	@-rm *.o *.d kernel.elf kernel.img .depend > /dev/null 2>&1 || true

run: kernel.elf
	${QEMU} ${QEMU_ARGS} -kernel kernel.elf -initrd runtime.elf,Tss.o 2>&1 | tee run.log

run-smp: kernel.elf
	${QEMU} ${QEMU_ARGS} -smp 4 -kernel kernel.elf 2>&1 | tee run.log

run-grub: kernel.img
	${QEMU} -fda kernel.img

TAGS: ${SRC}
	${HIDE} ctags -e -R ${BUILD_ROOT}
