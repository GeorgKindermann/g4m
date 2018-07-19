CC = g++ -Wall -O3 -funroll-loops -march=native -std=c++0x -mtune=native
G4MPATH = /home/georg/prg/g4m/increment/

all:	 2getTempPrec 4g4mSiteVal

2getTempPrec:	2getTempPrec.cc
	g++ 2getTempPrec.cc -o2getTempPrec -lnetcdf_c++ -lnetcdf -O3 -funroll-loops -march=native -mtune=native -std=c++11 -Wall

4g4mSiteVal:	4g4mSiteVal.cc
	$(CC) 4g4mSiteVal.cc -o4g4mSiteVal $(G4MPATH)mai.o $(G4MPATH)increment.o  $(G4MPATH)ageStruct.o
	strip 4g4mSiteVal
