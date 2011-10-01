CFLAGS=-Wall

all: src/micetrap

src/micetrap: src/server.o

clean:
		rm -f src/micetrap
		rm -f src/server.o
