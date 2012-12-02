CC = g++ -std=c++0x


sample: sample.cc pretty_printing.h position.h
	$(CC) sample.cc -o sample

test: test.cc pretty_printing.h position.h
	$(CC) test.cc -o test

fuzzer: fuzzer.cc pretty_printing.h position.h
	$(CC) fuzzer.cc -o fuzzer

.PHONY:
run_tests: test fuzzer
	./test  >test.golden
	./fuzzer

clean:
	rm sample test fuzzer
