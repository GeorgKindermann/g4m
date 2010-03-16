#CC = g++-4.4 -Wall -O3 -funroll-loops -march=core2
CC = g++-4.4 -Wall -O3 -funroll-loops -march=core2 -pg

all:	ic mi as

ic:	increment.o testIncrement

increment.o:	increment.cc increment.h misc.o
	$(CC) increment.cc -c

testIncrement:	testIncrement.cc increment.o
	$(CC) testIncrement.cc -otestIncrement increment.o misc.o
	strip testIncrement

mi:	misc.o testMisc

misc.o:	misc.cc misc.h
	$(CC) misc.cc -c

testMisc:	testMisc.cc misc.o
	$(CC) testMisc.cc -otestMisc misc.o
	strip testMisc

as:	ageStruct.o testAgeStruct

ageStruct.o:	ageStruct.cc ageStruct.h
	$(CC) ageStruct.cc -c

testAgeStruct:	testAgeStruct.cc ageStruct.o increment.o misc.o
	$(CC) testAgeStruct.cc -otestAgeStruct ageStruct.o increment.o misc.o
	strip testAgeStruct




allold:	 mi ic as  co



co:	mi ic as testConsistent.cc
	g++ -Wall testConsistent.cc -O2 -otestConsistent ageStruct.o increment.o misc.o
	strip testConsistent
