CC = gcc

CFLAGS = -Wall -g -std=c11

LDLIBS = -lm

ALL = test
all : $(ALL)

projet2019.o : projet2019.c projet2019.h

test.o : test_00.c projet2019.h

test : test_00.o projet2019.o

cleanall : rm -rf *~ *.o $(ALL)
