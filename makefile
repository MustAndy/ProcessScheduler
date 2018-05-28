#
# makefile for data security 
# wang,jingqing
# 30/03/2018
#
CC = gcc

CFLAGS = -Wall

OBJS = main.o phase2.o phase1.o util.o

scheduler: $(OBJS)
	$(CC) $(CFLAGS) -o scheduler $(OBJS)

main.o:header.h
	$(CC) $(CFLAGS) -c main.c

phase2.o:header.h
	$(CC) $(CFLAGS) -c phase2.c 
	
phase1.o:header.h
	$(CC) $(CFLAGS) -c phase1.c

util.o:header.h
	$(CC) $(CFLAGS) -c util.c

clean:
	-rm scheduler $(OBJS)