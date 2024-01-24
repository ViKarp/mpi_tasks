#!/bin/sh
#module add mpi/openmpi-local
module add openmpi
mpic++ task1.cpp -o run
