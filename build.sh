#!/bin/sh

if [ $(uname) = "NetBSD" ]
then
    MAKE=make
else
    MAKE=bmake
fi

export HAS_BUILD_SH=1
export MACHINE=i686
export MAKEOBJDIR=$PWD/obj.${MACHINE}

mkdir -p ${MAKEOBJDIR}

${MAKE} depend
${MAKE} "$@"
