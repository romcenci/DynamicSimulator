#!/bin/bash

MODE=0
L=500

while getopts "l:m:" opt; do
    case $opt in
	l)  L=$OPTARG ;;
	m)  MODE=$OPTARG ;;
	\?) exit 1 ;;
	:)  exit 1 ;;
    esac
done

gcc DynSimOpenGL.c -lm -lGL -lglfw -Dmo=$MODE -DTAM=$L -o DynSim
./DynSim

wait
rm DynSim
