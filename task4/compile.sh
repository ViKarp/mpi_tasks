#!/bin/sh
#module add mpi/openmpi-local
module add openmpi
mpic++ task4.cpp -o run
