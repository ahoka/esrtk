#!/bin/sh

if [ $(uname) = "NetBSD" ]
then
    MAKE=make
else
    MAKE=bmake
fi

export HAS_BUILD_SH=1
export MACHINE=i686
export MAKEOBJDIR=$PWD/scan.${MACHINE}

mkdir -p ${MAKEOBJDIR}

${MAKE} clean
${MAKE} depend

scan-build --use-c++=${CROSS}g++ --use-cc=${CROSS}gcc ${MAKE} all

${MAKE} clean

rm -rf ${MAKEOBJDIR}
