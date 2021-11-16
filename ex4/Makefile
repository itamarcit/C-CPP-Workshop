CC = gcc

CCFLAGS = -c -Wall -Wextra -Wvla -Werror -g -std=c99

LDFLAGS = -lm

.PHONY = all clean

clean:
	rm -f hashmap.o pair.o vector.o libhashmap.a libhashmap_tests.a

all: libhashmap.a libhashmap_tests.a

hashmap.o: hashmap.c vector.h pair.h
	$(CC) $(CCFLAGS) $< -o $@

pair.o: pair.c
	$(CC) $(CCFLAGS) $< -o $@

vector.o: vector.c
	$(CC) $(CCFLAGS) $< -o $@

test_suite.o: test_suite.c hashmap.c test_pairs.h hash_funcs.h \
			test_suite.h hashmap.h vector.c vector.h
	$(CC) $(CCFLAGS) $< -o $@

libhashmap.a: hashmap.o vector.o pair.o
	ar rcs $@ $^

libhashmap_tests.a: test_suite.o
	ar rcs $@ $^