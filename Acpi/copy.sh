#!/bin/sh

if [ $# -lt 1 ]
then
    echo Invalid number of arguments
    exit
fi

for i in dispatcher events executer hardware parser namespace utilities tables resources
do
    cp -r $1/source/components/$i/* Source/
done

cp -r $1/source/include/* Include/
