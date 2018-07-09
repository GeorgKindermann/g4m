CC = g++ -Wall -O3 -funroll-loops -march=native -std=c++11
G4MPATH = /home/georg/prg/g4m/increment/

all:	2calcStdBm 4calcBm

2calcStdBm:	2calcStdBm.cc $(G4MPATH)increment.o $(G4MPATH)misc.o
	$(CC) 2calcStdBm.cc -o2calcStdBm $(G4MPATH)increment.o $(G4MPATH)misc.o
	strip 2calcStdBm

4calcBm:	4calcBm.cc $(G4MPATH)increment.o $(G4MPATH)misc.o
	$(CC) 4calcBm.cc -o4calcBm $(G4MPATH)increment.o $(G4MPATH)misc.o
	strip 4calcBm
