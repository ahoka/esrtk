#!/bin/sh

if [ -z $1 ]
then
        exit
fi

ADDR=$(grep -m 1 -A 1 'Page fault:' $1  | tail -n 1 | cut -d " " -f 4)
LINE=$(i686-elf-addr2line -e kernel.elf $ADDR)

grep -m 1 -A 1 'Page fault:' $1
echo
echo "Code line is:"
echo $LINE

