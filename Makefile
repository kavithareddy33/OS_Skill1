CC = gcc
CFLAGS = -Wall -Wextra

prog2: src/prog2.c
	$(CC) $(CFLAGS) src/prog2.c -o src/prog2

clean:
	rm -f src/prog2 src/input.txt src/output.txt
