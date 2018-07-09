CC = g++ -Wall -O3 -funroll-loops -march=native -mtune=native
G4MPATH = ../

all:	2estimateNpp 4useAgestruct

2estimateNpp: 	2estimateNpp.cc
	$(CC) 2estimateNpp.cc -o2estimateNpp $(G4MPATH)mai.o
	strip 2estimateNpp

4useAgestruct:	4useAgestruct.cc
	$(CC) 4useAgestruct.cc -o4useAgestruct $(G4MPATH)ageStruct.o $(G4MPATH)increment.o $(G4MPATH)misc.o
	strip 4useAgestruct
