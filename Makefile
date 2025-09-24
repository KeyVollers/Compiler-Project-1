CC = g++
LEVEL = -g
CFLAGS = -Wall -Wextra -Werror -fno-exceptions -fno-rtti $(LEVEL)

test_scanner: test_scanner.cpp scanner.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f test_scanner

all: test_scanner

.PHONY: clean all
