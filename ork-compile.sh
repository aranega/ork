#!/bin/bash
# Simple dumb script

ORKPATH=$(pwd)

file=$(basename "$1")
filename="${file%.*}"

./ork ${file} > ${filename}.cc
g++ ${filename}.cc -I${ORKPATH} ${ORKPATH}/libork.a -o ${filename}
