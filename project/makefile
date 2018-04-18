#
# makefile for data security 
# wang,jingqing
# 30/03/2018
#
CC = gcc

CFLAGS = -Wall

OBJS = main.o scheduler.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS)

main.o:header.h
	$(CC) $(CFLAGS) -c main.c

scheduler.o:header.h
	$(CC) $(CFLAGS) -c scheduler.c 
	
clean:
	-rm main $(OBJS)