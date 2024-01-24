#!/bin/sh
#module add mpi/openmpi-local
module add openmpi
mpic++ task3.cpp -o run
