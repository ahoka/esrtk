#!/bin/sh

if [ $(uname) = "NetBSD" ]
then
    MAKE=make
else
    MAKE=bmake
fi

MAKE_FLAGS="-r"

export HAS_BUILD_SH=1
export MACHINE=i686
export MAKEOBJDIR=$PWD/obj.${MACHINE}

mkdir -p ${MAKEOBJDIR}

${MAKE} ${MAKE_FLAGS} depend
${MAKE} ${MAKE_FLAGS} "$@"
${MAKE} ${MAKE_FLAGS} -f Runtime/Makefile clean
${MAKE} ${MAKE_FLAGS} -f Runtime/Makefile
