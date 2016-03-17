CC=gcc
CFLAGS=--std=c99 -c -Wall -O2 -D_SVID_SOURCE=1 -D_XOPEN_SOURCE=700 -pthread
LDFLAGS=
LIBRARY = better_string.o err.o input_stream.o message_buffer.o message_queue.o mutex_and_signal.o safe_memory_operations.o safe_utility.o signals.o string_builder.o thread.o utility.o vector.o
MUZEUM = muzeum/delegate.o muzeum/muzeum.o muzeum/tables_operations.o
FIRMA = firma/activity_suspender.o firma/firma.o firma/worker.o
BANK = bank/bank.o

all: firma bank muzeum

.c.o: *.c *.h
	$(CC) $(CFLAGS) $< -o $@

firma: folder $(LIBRARY) $(FIRMA)
	$(CC) -Wall -O2 $(LIBRARY) $(FIRMA) -pthread -o build/firma

bank: folder $(LIBRARY) $(BANK)
	$(CC) -Wall -O2 $(LIBRARY) $(BANK) -pthread -o build/bank

muzeum: folder $(LIBRARY) $(MUZEUM)
	$(CC) -Wall -O2 $(LIBRARY) $(MUZEUM) -pthread -o build/muzeum

.PHONY: clean
clean:
	rm -f *.o
	rm -f **/*.o
	rm -f build/firma build/bank build/muzeum
	rmdir build

.PHONY: folder
folder:
	mkdir -p build
