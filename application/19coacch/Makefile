CC = g++ -Wall -O2 -march=native -mtune=native -std=c++17
G4MPATH = ~/prg/g4m/increment/

all:	3estimateNpp 5useAgestruct

3estimateNpp: 	3estimateNpp.cc
	$(CC) 3estimateNpp.cc -o3estimateNpp $(G4MPATH)mai.o
	strip 3estimateNpp

5useAgestruct:	5useAgestruct.cc
	$(CC) 5useAgestruct.cc -o5useAgestruct $(G4MPATH)ageStruct.o $(G4MPATH)increment.o $(G4MPATH)misc.o
	strip 5useAgestruct
