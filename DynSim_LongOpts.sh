#!/bin/bash

MODE=1
L=500
HEIGHT=660
WIDTH=880

while getopts "l:m:-:" opt; do
    case $opt in
	l)  L=$OPTARG ;;
	m)  MODE=$OPTARG ;;
	-)  LONG_OPTARG="${OPTARG#*=}"
            case $OPTARG in
		height=?*)  HEIGHT=$LONG_OPTARG ;;
		height*  )  echo "No argument for --$OPTARG option" >&2; exit 2 ;;
		width=?* )  WIDTH=$LONG_OPTARG ;;
		width*   )  echo "No argument for --$OPTARG option" >&2; exit 2 ;;
		'' )        break ;;
		* )         echo "Illegal option --$OPTARG" >&2; exit 2 ;;
            esac ;;
	\?) exit 1 ;;
	:)  exit 1 ;;
    esac
done

gcc libbmp/libbmp.c DynSimOpenGL.c -lm -lGL -lglfw -Dmo=$MODE -DTAM=$L -DWINDOWS_WIDTH=$WIDTH -DWINDOWS_HEIGHT=$HEIGHT -o DynSim
./DynSim

wait
rm DynSim
