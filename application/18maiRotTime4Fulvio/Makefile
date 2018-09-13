CC = g++ -Wall -O3 -march=native

all:	ic mi

ic:	increment.o 2getUMaiBm

increment.o:	increment.cc increment.h misc.o
	$(CC) increment.cc -c

2getUMaiBm:	2getUMaiBm.cc increment.o misc.o
	$(CC) 2getUMaiBm.cc -o2getUMaiBm increment.o misc.o

mi:	misc.o

misc.o:	misc.cc misc.h
	$(CC) misc.cc -c
