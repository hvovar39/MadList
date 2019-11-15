CC = gcc

CFLAGS = -Wall -g -std=c11

LDLIBS = -lm

ALL = projet2019
all : $(ALL)


projet2019 : projet2019.C

cleanall : rm -rf *~ $(ALL)
