CC=gcc
CFLAGS=-Wall -std=c99 -g -O2 -o

all: codes

codes: dp.c greedy.c naive.c
	$(CC) $(CFLAGS) dp dp.c
	$(CC) $(CFLAGS) greedy greedy.c
	$(CC) $(CFLAGS) naive naive.c

clean:
	rm dp greedy naive

