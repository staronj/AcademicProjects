CC=gcc
CFLAGS=-std=c99 -c -Wall -O2
LDFLAGS=
LIBRARY = input_stream.o err.o better_string.o process_handle.o safe_memory_operations.o string_builder.o to_onp_algorithm.o vector.o

all: W ToONP

W.o: W.c
	$(CC) $(CFLAGS) W.c -o W.o

ToONP.o: ToONP.c
	$(CC) $(CFLAGS) ToONP.c -o ToONP.o

.c.o: *.c *.h
	$(CC) $(CFLAGS) $< -o $@

W: $(LIBRARY) W.o
	$(CC) -Wall -O2 $(LIBRARY) W.o -o W

ToONP: $(LIBRARY) ToONP.o
	$(CC) -Wall -O2 $(LIBRARY) ToONP.o -o ToONP

.PHONY: clean
clean:
	rm -f *.o W ToONP