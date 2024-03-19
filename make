OBJS	= spchk.o
SOURCE	= spchk.c
HEADER	= 
OUT	= spchk
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

spchk.o: spchk.c
	$(CC) $(FLAGS) spchk.c 


clean:
	rm -f $(OBJS) $(OUT)
