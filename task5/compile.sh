#!/bin/sh
#module add mpi/openmpi-local
module add openmpi
mpic++ task5.cpp -o run
