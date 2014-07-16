#!/bin/sh

export HAS_BUILD_SH=1
export MACHINE=i686
export MAKEOBJDIR=$PWD/obj.${MACHINE}
mkdir -p ${MAKEOBJDIR}
bmake "$@"
