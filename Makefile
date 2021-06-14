CC:=gcc
FLAGS:=-O3
OBJS:=main.o set.o ram.o cache.o

main: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o main -lm

%.o: %.c || %.h
	$(CC) $(FLAGS) -c $< -o $@

run: main
	./main -L1s 4 -L1E 1 -L1b 4 -L2s 8 -L2E 2 -L2b 4 -t test_large.trace -r RAM.dat

clean:
	rm -f *.o main L1D.txt L1I.txt L2.txt updated_ram.dat