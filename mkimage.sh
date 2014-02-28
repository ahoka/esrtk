#!/bin/sh

if [ $# -lt 2 ]
then
    echo not enough arguments
    exit 1
fi

if [ ! -e $1 ]
then
    echo invalid input
    exit 1
fi

# TODO mktemp

dd if=/dev/zero of=pad bs=1 count=750

cat Loader/stage1 Loader/stage2 pad $1 > tmp.img

pad2size=$((1440 * 1024 - $(wc -c < tmp.img | tr -s ' ' | cut -d ' ' -f2)))
dd if=/dev/zero of=pad2 bs=1 count=${pad2size}

cat tmp.img pad2 > $2

rm pad tmp.img pad2
