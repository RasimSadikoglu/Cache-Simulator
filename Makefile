CC:=gcc
FLAGS:=-g -Wall -Wextra -Wpedantic
OBJS:=main.o set.o ram.o cache.o

main: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o main -lm

%.o: %.c || %.h
	$(CC) $(FLAGS) -c $< -o $@

run: main
	./main -L1s 3 -L1E 2 -L1b 4 -L2s 8 -L2E 4 -L2b 4 -t test_large.trace

clean:
	rm -f *.o main