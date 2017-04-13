CC = g++ -Wall -O3 -funroll-loops -march=native -std=c++0x -mtune=native

all:	ic mi as ma

ic:	increment.o testIncrement

increment.o:	increment.cc increment.h misc.o
	$(CC) increment.cc -c

testIncrement:	testIncrement.cc increment.o
	$(CC) testIncrement.cc -otestIncrement increment.o misc.o

mi:	misc.o testMisc

misc.o:	misc.cc misc.h
	$(CC) misc.cc -c

testMisc:	testMisc.cc misc.o
	$(CC) testMisc.cc -otestMisc misc.o

as:	ageStruct.o testAgeStruct

ageStruct.o:	ageStruct.cc ageStruct.h
	$(CC) ageStruct.cc -c

testAgeStruct:	testAgeStruct.cc ageStruct.o increment.o misc.o
	$(CC) testAgeStruct.cc -otestAgeStruct ageStruct.o increment.o misc.o

ma:	mai.o testMai

mai.o:	mai.cc mai.h
	$(CC) mai.cc -c

testMai: testMai.cc mai.o
	$(CC) testMai.cc -otestMai mai.o





co:	mi ic as testConsistent.cc
	g++ -Wall testConsistent.cc -O2 -otestConsistent ageStruct.o increment.o misc.o
	strip testConsistent
