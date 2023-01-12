include ../../../common/Makefile

CC=mpicc
XC=mpic++
#adding -fopenmp to flag to enable openmp
FLAG=-I../../../common/ -fopenmp

# macos: brew install gcc
ifneq (, $(shell which gcc-11))
	CC=MPICH_CC=gcc-11 mpicc
	XC=MPICH_CXX=g++-11 mpic++
endif