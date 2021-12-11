CC?=gcc
SRC:=src/main.c src/ram.c src/cache.c src/set.c src/pargser.c

bin/main: bin/ output/ rams/ $(SRC)
	$(CC) $(FLAGS) $(SRC) -o bin/main -lm

bin/:
	mkdir -p bin

output/:
	mkdir -p output

rams/:
	mkdir -p rams

run: bin/main
	bin/main -L1s 4 -L1E 1 -L1b 4 -L2s 8 -L2E 2 -L2b 4 -t traces/test_small.trace -r rams/RAM.dat

clean:
	rm -rf output bin