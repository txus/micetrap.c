CFLAGS=-Wall

all: src/micetrap

src/micetrap: src/utils.o src/server.o

clean:
		rm -f src/micetrap
		rm -f src/server.o
		rm -f src/utils.o
