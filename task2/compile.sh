#!/bin/sh
#module add mpi/openmpi-local
module add openmpi
mpic++ task2.cpp -o run
