CC = gcc
CFLAGS = -Wall -Wextra -g

all: practical1 practical2 practical3

practical1: src/practical1.c
	$(CC) $(CFLAGS) src/practical1.c -o src/practical1

practical2: src/practical2.c
	$(CC) $(CFLAGS) src/practical2.c -o src/practical2

practical3: src/practical3.c
	$(CC) $(CFLAGS) src/practical3.c -o src/practical3

clean:
	rm -f src/practical1 src/practical2 src/practical3
