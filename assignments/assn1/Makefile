CC = clang
CFLAGS = -g -fno-omit-frame-pointer -fsanitize=address

all: assn1 unittest

assn1: assn1.c
	$(CC) $(CFLAGS) assn1.c -o assn1

unittest: assn1.c unittest.c
	$(CC) $(CFLAGS) -I/opt/homebrew/include -L/opt/homebrew/lib -lcmocka unittest.c -o unittest
