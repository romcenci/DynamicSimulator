#!/bin/bash

   
gcc DynSimOpenGL.c -lm -lGL -lglfw -Dmo=$1 -o DynSim
./DynSim