#!/bin/sh

#
# Wrap the wrapper to be compatible with buildbot
#

if [ $1 = "autobuild" ]
then
    make autobuild
    cd Test && make check
fi

make "$@"
