CC = g++ -std=c++0x

sample: sample.cc pretty_printing.h  position.h
	$(CC) sample.cc -o sample
