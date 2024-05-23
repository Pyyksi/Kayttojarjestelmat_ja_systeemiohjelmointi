#Viikko 9 tehtävä 1. Makefile
#Apua makefileen: makefiletutorial.com

CC = gcc
CFLAGS= -Wall
DEPS = header_file.h
OBJ = v9main.o function.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

calc_sum: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
    
clean:
	rm -f *.o calc_sum
	
