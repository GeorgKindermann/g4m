CC = g++ -Wall -O3 -funroll-loops -march=native -std=c++11 -mtune=native
G4MPATH = /home/georg/prg/g4m/increment/

all:    4g4gm 5g4gm

4g4gm:	4g4gm.cc
	$(CC) 4g4gm.cc -o4g4gm $(G4MPATH)ageStruct.o $(G4MPATH)increment.o $(G4MPATH)misc.o $(G4MPATH)mai.o
	strip 4g4gm

5g4gm:	5g4gm.cc
	$(CC) 5g4gm.cc -o5g4gm $(G4MPATH)ageStruct.o $(G4MPATH)increment.o $(G4MPATH)misc.o $(G4MPATH)mai.o
	strip 5g4gm
