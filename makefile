#
# makefile for data security 
# wang,jingqing
# 30/03/2018
#
CC = gcc

CCP = g++

CFLAGS = #-Wall

OBJS = main.o scheduler.o

main: $(OBJS)
	$(CCP) $(CFLAGS) -o main $(OBJS)

main.o:main.cpp
	$(CCP) $(CFLAGS) -c main.cpp

scheduler.o:scheduler.c
	$(CC) $(CFLAGS) -c scheduler.c 
	
clean:
	-rm main $(OBJS)