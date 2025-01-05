CC = gcc
CFLAGS = -O3 -march=native -Wall -pedantic -std=c17 -fopenmp
HEADERS = ppm.h scene.h
OBJ = main.o ppm.o scene.o 

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $<

.PHONY: clean

clean:
	rm -f *.o
	rm -f main
