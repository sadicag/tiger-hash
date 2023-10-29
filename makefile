CC=gcc
ODIR=bin/tiger-hash
CFLAGS=-lncurses

all: main.c
	$(CC) -o $(ODIR) main.c $(CFLAGS) 

clean:
	rm -f $(ODIR)
