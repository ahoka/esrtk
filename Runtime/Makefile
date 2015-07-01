# Makefile for ESRTK
#

.ifndef HAS_BUILD_SH
.error "Please run the build using build.sh"
.endif

BUILD_ROOT := ${.CURDIR}
PLATFORM=	X86

FIND:=		find

BUILD_HOST!=	uname

CROSS=		i686-elf-

AS=		${CC}
LD=		${CROSS}g++
SIZE=		${CROSS}size

COPTS+=		-O2 -march=i686 -g \
		-Wall -Wextra \
		-nostdlib -nostdinc -fno-builtin \
		-fno-omit-frame-pointer \
		-fno-stack-protector -fpic

LDFLAGS=	${COPTS} -r
AFLAGS+=	-g ${COPTS}
CFLAGS=		-std=c11 ${COPTS}
CXXFLAGS=	-std=c++14 ${COPTS} -fno-exceptions -fno-rtti

CC=		${CROSS}gcc
CXX=		${CROSS}g++
CPP=		${CROSS}cpp -m32 -nostdinc
GCC_VERSION!=	${CC} -dumpversion

.if ${GCC_VERSION:C/^4.9//} != ${GCC_VERSION}
COPTS+=		-fdiagnostics-color=always
.endif

CPPFLAGS+=	-D__esrtk__
#CPPFLAGS+=	-D_BSD_SOURCE

SRCDIR=		StandardLibrary

SRC!=		(cd ${BUILD_ROOT} && find ${SRCDIR} \( -name '*.c' -or -name '*.cc' -or -name '*.cpp' -or -name '*.S' \) -exec basename {} \;)
DIR!=		(cd ${BUILD_ROOT} && find ${SRCDIR} \( -name '*.c' -or -name '*.cc' -or -name '*.cpp' -or -name '*.S' \) -exec dirname {} \; | sort -u)

OFILES=		${SRC:C/\.(cc|cpp|c|S)$/.o/}
DFILES=		${SRC:C/\.(cc|cpp|c|S)$/.d/}

.PATH:		${DIR:S/^/${BUILD_ROOT}\//}

#HIDE=	@
.SUFFIXES:	.c .cc .cpp .S .d .o

all:	runtime.elf

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

.PHONY: clean
clean:
	-rm *.o runtime.elf

runtime.elf: ${OFILES}
	@echo Linking kernel executable
	${HIDE} ${LD} ${LDFLAGS} -T ${BUILD_ROOT}/linker.ld -o ${.TARGET} ${.ALLSRC}
	@${SIZE} $@
