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

AS=		${CROSS}as
LD=		${CROSS}ld
SIZE=		${CROSS}size

QEMU=		qemu-system-i386

LDFLAGS=	-melf_i386

ASFLAGS+=	--32 -g

COPTS+=		-O2 -march=i686 -m32 -g3 \
		-Wall -Wextra -Werror \
		-nostdlib -nostdinc -fno-builtin \
		-fno-stack-protector

CFLAGS=		-std=c99 ${COPTS}

CXXFLAGS=	${COPTS} -fno-exceptions -fno-rtti

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
CPP=		${CROSS}gcc -m32 -nostdinc -E
GCC_VERSION!=	${CC} -dumpversion

.if ${GCC_VERSION} == "4.9.1"
COPTS+=		-fdiagnostics-color=always
.endif

.endif

CXXFLAGS+=	-std=c++11

CPPFLAGS+=	-I${BUILD_ROOT}/Include
CPPFLAGS+=	-I${BUILD_ROOT}/Include/Kernel
CPPFLAGS+=	-I${BUILD_ROOT}/CInclude
CPPFLAGS+=	-I${BUILD_ROOT}/CxxInclude
CPPFLAGS+=	-I${BUILD_ROOT}/Templates
CPPFLAGS+=	-I${BUILD_ROOT}/CxxInclude/ustl

# XXX unify with the above through transformation
INCDIRS=	${BUILD_ROOT}/Include ${BUILD_ROOT}/CInclude \
		${BUILD_ROOT}/CxxInclude ${BUILD_ROOT}/Templates \
		${BUILD_ROOT}/CxxInclude/ustl

# XXX these should be only provided for Standard
#
CPPFLAGS+=	-I${BUILD_ROOT}/BsdCompat
CPPFLAGS+=	-DHAVE_NBTOOL_CONFIG_H=0

CPPFLAGS+=	-DHAVE_STRLCAT=0 -DHAVE_STRSEP=0 -DHAVE_STRLCPY=0 -D__ELF__

CPPFLAGS+=	-D__esrtk__

CFLAGS+=	${CPPFLAGS}
CXXFLAGS+=	${CPPFLAGS}

SRCDIR=		Supervisor CLibrary CxxLibrary Drivers Kernel FileSystem Hal Loader
TESTDIR=	Test

TCCFILES:=	${FIND} ${TESTDIR} -name '*.cc'

SRC!=		(cd ${BUILD_ROOT} && find ${SRCDIR} \( -name '*.c' -or -name '*.cc' -or -name '*.S' \) -exec basename {} \;)
DIR!=		(cd ${BUILD_ROOT} && find ${SRCDIR} \( -name '*.c' -or -name '*.cc' -or -name '*.S' \) -exec dirname {} \; | sort -u)

OFILES=		${SRC:C/\.(cc|c|S)$/.o/}
DFILES=		${SRC:C/\.(cc|c|S)$/.d/}

.PATH:		${DIR:S/^/${BUILD_ROOT}\//}

#HIDE=	@
.SUFFIXES:	.c .cc .S .d

all:	MultiLoader.o kernel.elf kernel.img

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
	cppcheck -q -i ${INCDIRS} ${SRCDIR}

# .cc.o:
# 	@echo Compiling $<
# 	${HIDE} ${CXX} ${CXXFLAGS} -c -o $*.o $*.cc

# .c.o:
# 	@echo Compiling $<
# 	${HIDE} ${CC} ${CFLAGS} -c -o $*.o $*.c

# .S.o:
# 	@echo Compiling $<
# 	${HIDE} ${CPP} ${CPPFLAGS} -DASSEMBLER $*.S | ${AS} ${ASFLAGS} -o $*.o

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

kernel.elf: MultiLoader.o ${OFILES}
	@echo Linking kernel executable
	${HIDE} ${LD} ${LDFLAGS} -T ${BUILD_ROOT}/Kernel/${PLATFORM}/linker.ld -o ${.TARGET} ${.ALLSRC}
	@${SIZE} $@

kernel.img: kernel.elf
	@echo Building kernel image
	env BUILD_ROOT=${BUILD_ROOT:Q}/ ${BUILD_ROOT}/mkimage.sh kernel.elf kernel.img

clean:
	@-rm ${.OBJDIR}/*.o kernel.elf kernel.img .depend > /dev/null 2>&1 || true

run-isa: kernel.elf
	${QEMU} -cpu pentium2 -M isapc -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset

run-pc: kernel.elf
	${QEMU} -M pc -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset

run: kernel.elf
	${QEMU} -M q35 -watchdog i6300esb -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset 2>&1 | tee run.log

run-smp: kernel.elf
	${QEMU} -M q35 -smp 2 -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset 2>&1 | tee run.log

run-debug: kernel.elf
	${QEMU} -M q35 -net none -kernel kernel.elf -boot order=c -serial stdio -s -S 2>&1 | tee run.log

serial: kernel.elf
	${QEMU} -net none -kernel kernel.elf -boot order=c -serial stdio -d cpu_reset -nographic

monitor: kernel.elf
	${QEMU} -net none -kernel kernel.elf -boot order=c -monitor stdio

run-grub: kernel.img
	${QEMU} -fda kernel.img

TAGS: ${SRC}
	${HIDE} ctags -e -R ${BUILD_ROOT}
