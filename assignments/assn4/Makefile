CC = clang
CFLAGS = -g -fno-omit-frame-pointer -fsanitize=address
DEPS = functions.h

all: assn4 unittest

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

assn4: assn4.o functions.o
	$(CC) $(CFLAGS) $^ -o $@

unittest: functions.o unittest.o
	$(CC) $(CFLAGS) -I/opt/homebrew/include -L/opt/homebrew/lib -lcmocka $^ -o $@
