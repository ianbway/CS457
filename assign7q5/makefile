# Ian Braudaway
# CS457-002
# Makefile for Assignment 7 Question 5

IWBRAUDAWAY_OBJS = string.o scanner.o da.o iwbraudaway.o
DA_OBJS = string.o da.o
OPTS = -Wall -Wextra -std=c99 -g -c 
LOPTS = -Wall -Wextra -std=c99 -g
CC = gcc

all : iwbraudaway

iwbraudaway : $(IWBRAUDAWAY_OBJS)
	$(CC) $(LOPTS) $(IWBRAUDAWAY_OBJS) -o iwbraudaway

iwbraudaway.o : iwbraudaway.c 
	$(CC) $(OPTS) iwbraudaway.c	

scanner.o : scanner.c scanner.h
	$(CC) $(OPTS) scanner.c

string.o : string.c string.h
	$(CC) $(OPTS) string.c

da.o : da.c da.h
	$(CC) $(OPTS) da.c

clean :
	rm -f $(DA_OBJS) $(IWBRAUDAWAY_OBJS) ./iwbraudaway

test :
	./iwbraudaway
