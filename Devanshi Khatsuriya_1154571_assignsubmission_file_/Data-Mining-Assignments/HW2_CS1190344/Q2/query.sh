#!/bin/bash

# usage: ./query.sh

# g++ -c -I/home/cse/btech/cs1190344/scratch/COL761_HW2_HPC/Data-Mining-Assignments-Dev/HW2/Q2_vf2/vf2lib/include query.cpp
# g++ -o query query.o -L/home/cse/btech/cs1190344/scratch/COL761_HW2_HPC/Data-Mining-Assignments-Dev/HW2/Q2_vf2/vf2lib/lib -lvf -lstdc++ -lm
g++ -w -O3 -c query.cpp
g++ -w -O3 -o query query.o -L/home/cse/btech/cs1190344/scratch/COL761_HW2_HPC/Data-Mining-Assignments-Dev/HW2/Q2_vf2/vf2lib/lib -lvf -lstdc++ -lm
./query
rm query.o 
rm query