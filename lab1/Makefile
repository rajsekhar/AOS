
# include files
INCLUDE = -I .

# compiler
CC = gcc -g -fPIC -DDEBUG $(INCLUDE)

STATICLIB = mymalloc
LINKLIB = -L. -lmymalloc

# archived - static library
AR = ar

all: mymalloc main

mymalloc.o: mymalloc.c mymalloc.h
			$(CC) -c mymalloc.c -o mymalloc.o

mymalloc: mymalloc.o
	$(AR) rcs libmymalloc.a mymalloc.o

main: libmymalloc.a
	$(CC) main.c  $(LINKLIB) -o main 

clean:
	rm -vfr *.o *.a main