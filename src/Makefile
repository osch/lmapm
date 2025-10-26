# Makefile for building mapm library and test program
# mapm was avaliable at http://www.tc.umn.edu/~ringx004/mapm-main.html

CC= gcc
CFLAGS= -pedantic -Wall -Wextra -O2 $(PIC)
L= libmapm.a
T= validate

all:	$L

$L:	mapm*.c *.h
	$(CC) $(CFLAGS) -c mapm*.c
	$(CC) $(CFLAGS) -c -O3 mapmutil.c mapmfmul.c
	ar rcu $L mapm*.o
	-rm -f *.o

test:	$T
	./$T

$T:	$T.c $L
	$(CC) $(CFLAGS) -o $T $T.c $L -lm

clean:
	-rm -f *.o *.a $T
