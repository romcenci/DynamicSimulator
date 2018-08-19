#!/bin/bash

MODE=1
L=500
HEIGHT=500
WIDTH=700

for arg in "$@"; do
  shift
  case "$arg" in
    "--mode")   set -- "$@" "-m" ;;
    "--height") set -- "$@" "-h" ;;
    "--width")  set -- "$@" "-w" ;;
    *)          set -- "$@" "$arg"
  esac
done

while getopts "l:m:h:w:" opt; do
    case $opt in
	l)  L=$OPTARG ;;
	m)  MODE=$OPTARG ;;
	h)  HEIGHT=$OPTARG ;;
	w)  WIDTH=$OPTARG ;;
	\?) exit 1 ;;
	:)  exit 1 ;;
    esac
done

gcc libbmp/libbmp.c DynSimOpenGL.c -lm -lGL -lglfw -Dmo=$MODE -DTAM=$L -DWINDOWS_WIDTH=$WIDTH -DWINDOWS_HEIGHT=$HEIGHT -o DynSim
./DynSim

wait
rm DynSim
