CC = g++ -Wall -O3 -funroll-loops -march=native -std=c++11 -mtune=native
G4MPATH = /home/georg/prg/g4m/increment/

all:    2g4gm

2g4gm:	2g4gm.cc
	$(CC) 2g4gm.cc -o2g4gm $(G4MPATH)ageStruct.o $(G4MPATH)increment.o $(G4MPATH)misc.o $(G4MPATH)mai.o
	strip 2g4gm
