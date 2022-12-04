CC = clang
CFLAGS = -g -fno-omit-frame-pointer -fsanitize=address
DEPS = functions.h
SRCS = assn4.c functions.c
OBJS = assn4.o functions.o

all: assn4 unittest

assn4: $(OBJS)
	$(CC) $(CFLAGS) -fuse-ld=lld $(OBJS) -o $@

unittest: functions.o unittest.c
	$(CC) $(CFLAGS) -fuse-ld=lld -I/opt/homebrew/include -L/opt/homebrew/lib -lcmocka $^ -o $@

clean:
	rm -rf $(OBJS) unittest unittest.dSYM assn4 assn4.dSYM
