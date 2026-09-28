CC = gcc
CFLAGS = -Wall -Wextra

all: practical1 practical2 practical3 practical4 practical5 practical6 practical7

practical1: src/practical1.c
	$(CC) $(CFLAGS) src/practical1.c -o src/practical1

practical2: src/practical2.c
	$(CC) $(CFLAGS) src/practical2.c -o src/practical2

practical3: src/practical3.c
	$(CC) $(CFLAGS) src/practical3.c -o src/practical3

practical4: src/practical4.c
	$(CC) $(CFLAGS) src/practical4.c -o src/practical4

practical5: src/practical5.c
	$(CC) $(CFLAGS) src/practical5.c -o src/practical5

practical6: src/practical6.c
	$(CC) $(CFLAGS) src/practical6.c -o src/practical6

practical7: src/practical7.c
	$(CC) $(CFLAGS) src/practical7.c -o src/practical7

clean:
	rm -f src/practical1 src/practical2 src/practical3 src/practical4 src/practical5 src/practical6 src/practical7
