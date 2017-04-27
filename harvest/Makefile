CC = g++ -Wall -O3 -funroll-loops -march=core2

all:	ha tha

ha:	harvest.o

harvest.o:	harvest.h harvest.cc
	$(CC) harvest.cc -c

tha:	testHarvest.cc harvest.o
	$(CC) testHarvest.cc -otestHarvest harvest.o
	strip testHarvest
