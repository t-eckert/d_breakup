#!/bin/bash
#PBS -l select=1:ncpus=1:mpiprocs=1
#PBS -q s1
#PBS -N GEANT_SIM
#PBS -j oe
#PBS -m a
#PBS -m e
n2n_d run.mac
exit
