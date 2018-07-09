CC = g++ -Wall -O3 -funroll-loops -march=native -std=c++11

all:	y07_subroutine.o 2calcSoilc 4calcSoilcCal

y07_subroutine.o:	y07_subroutine.f90
	gfortran -c -O3 -funroll-loops -march=native y07_subroutine.f90

2calcSoilc:	2calcSoilc.cc y07_subroutine.o
	$(CC) 2calcSoilc.cc -o2calcSoilc y07_subroutine.o -lgfortran
	strip 2calcSoilc

4calcSoilcCal:	4calcSoilcCal.cc y07_subroutine.o
	$(CC) 4calcSoilcCal.cc -o4calcSoilcCal y07_subroutine.o -lgfortran
	strip 4calcSoilcCal
