CC = g++ -Wall -O3 -funroll-loops -march=native -mtune=native

all:	ic mi as ma

ic:	increment.o

increment.o:	increment.cc increment.h misc.o
	$(CC) increment.cc -c

mi:	misc.o

misc.o:	misc.cc misc.h
	$(CC) misc.cc -c

as:	ageStruct.o

ageStruct.o:	ageStruct.cc ageStruct.h
	$(CC) ageStruct.cc -c

ma:	mai.o

mai.o:	mai.cc mai.h
	$(CC) mai.cc -c
