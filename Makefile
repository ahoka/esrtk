# GNU Make wrapper for build system

MACHINE=i686
MAKEOBJDIR=${CURDIR}/obj.${MACHINE}

SYSMAKE=bmake
SYSMAKE_FLAGS=-r
SYSMAKEFILE=KernelMakefile
SYSMAKECMD=env HAS_BUILD_SH=1 MACHINE=i686 MAKEOBJDIR=${MAKEOBJDIR} ${SYSMAKE} ${SYSMAKE_FLAGS} -f ${SYSMAKEFILE}

all: build

${MAKEOBJDIR}:
	mkdir -p ${MAKEOBJDIR}

.PHONY: build
build: ${MAKEOBJDIR}
	${SYSMAKECMD} depend
	${SYSMAKECMD} all

.PHONY: run
run: build
	${SYSMAKECMD} run

.PHONY: clean
clean:
	${SYSMAKECMD} clean

.PHONY: autobuild
autobuild:
	${SYSMAKECMD} autobuild
