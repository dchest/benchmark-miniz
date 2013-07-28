#!/bin/sh

# build
cc -o c -O3 c.c -lz
if [ ! -f miniz.o ]; then
	cc -c -o miniz.o -O3 miniz.c
fi
cc -o m -O3 m.c miniz.o

test () {
	./c $1
	./m $1
}

for f in data/*
do
	test $f
done

rm c
rm m
