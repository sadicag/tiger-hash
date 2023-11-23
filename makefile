CC=gcc
ODIR=bin/tiger-hash

all: main.c
	$(CC) -o $(ODIR) main.c 

clean:
	rm -f $(ODIR)
