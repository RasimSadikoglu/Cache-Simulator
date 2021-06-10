CC:=gcc
FLAGS:=-g -Wall -Wextra -Wpedantic
OBJS:=main.o set.o ram.o cache.o

main: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o main -lm

%.o: %.c || %.h
	$(CC) $(FLAGS) -c $< -o $@

run: main
	./main -L1s 0 -L1E 0 -L1b 0 -L2s 0 -L2E 1 -L2b 0 -t test_medium.trace

clean:
	rm -f *.o main