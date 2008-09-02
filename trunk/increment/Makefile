all:	 mi ic as


mi:	testMisc misc.o

testMisc:	testMisc.cc misc.o
	g++ -Wall testMisc.cc -O2 -otestMisc misc.o
	strip testMisc

misc.o:	misc.cc misc.h
	g++ -Wall misc.cc -c -O2


as:	testAgeStruct ageStruct.o

testAgeStruct:	testAgeStruct.cc ageStruct.o increment.o misc.o
	g++ -Wall testAgeStruct.cc -O2 -otestAgeStruct ageStruct.o increment.o misc.o
	strip testAgeStruct

ageStruct.o:	ageStruct.cc ageStruct.h
	g++ -Wall ageStruct.cc -c -O2


ic:	testIncrement increment.o

testIncrement:	testIncrement.cc increment.o
	g++ -Wall testIncrement.cc -O2 -otestIncrement increment.o
	strip testIncrement

increment.o:	increment.cc increment.h
	g++ -Wall increment.cc -c -O2
