CC = g++ -std=c++0x


sample: sample.cc pretty_printing.h position.h
	$(CC) sample.cc -o sample

test: test.cc pretty_printing.h position.h
	$(CC) test.cc -o test

.PHONY:
run_test: test
	./test  >test.golden

clean:
	rm sample
