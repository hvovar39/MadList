CC = gcc

CFLAGS = -Wall -g -std=c11

LDLIBS = -lm

ALL = test_00
all : $(ALL)

projet2019.o : projet2019.c projet2019.h

affichage.o : affichage.c affichage.h projet2019.h

test_00.o : test_00.c projet2019.h affichage.h

test_00 : test_00.o projet2019.o affichage.o



clean :
	rm -rf *~

cleanall :
	rm -rf *~ *.o $(ALL)
