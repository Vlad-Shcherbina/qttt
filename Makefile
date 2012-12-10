CC = g++ -std=c++0x -O2


play: play.cc pretty_printing.h position.h
	$(CC) play.cc -o play

test: test.cc pretty_printing.h position.h
	$(CC) test.cc -o test

fuzzer: fuzzer.cc pretty_printing.h position.h
	$(CC) fuzzer.cc -o fuzzer

.PHONY:
run_tests: test fuzzer
	./test  >test.golden
	./fuzzer

clean:
	rm play test fuzzer
