#!/bin/sh

if [ $1 = "cc" ]
then
	rm unity_cc.cc || true
	find . ! -name 'unity_cc.cc' -name '*.cc' -exec echo \#include \"{}\" \; >> unity_cc.cc
fi

if [ $1 = "c" ]
then
	rm unity_c.c || true
	find . ! -name 'unity_c.c' -name '*.c' -exec echo \#include \"{}\" \; >> unity_c.c
fi

if [ $1 = "s" ]
then
	rm unity_s.S || true
	find . ! -name 'unity_s.S' ! -wholename "./Loader/*" -name '*.S' -exec echo \#include \"{}\" \; >> unity_s.S
fi
