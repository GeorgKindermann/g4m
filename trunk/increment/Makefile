all:	 as ic mi


mi:	testMisc misc

testMisc:	testMisc.cc misc.h
	g++ -Wall testMisc.cc -O2 -otestMisc misc.o
	strip testMisc

misc:	misc.cc misc.h
	g++ -Wall misc.cc -c -O2


as:	testAgeStruct #ageStruct

testAgeStruct:	testAgeStruct.cc ageStruct.o increment.o
	g++ -Wall testAgeStruct.cc -O2 -otestAgeStruct ageStruct.o increment.o
	strip testAgeStruct

ageStruct:	ageStruct.cc ageStruct.h
	g++ -Wall ageStruct.cc -c -O2


ic:	testIncrement #increment

testIncrement:	testIncrement.cc increment.o
	g++ -Wall testIncrement.cc -O2 -otestIncrement increment.o
	strip testIncrement

increment:	increment.cc increment.h
	g++ -Wall increment.cc -c -O2
