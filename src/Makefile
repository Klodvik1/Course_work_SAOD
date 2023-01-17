CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++11
TARGET = main

all: clean main

main: clean
	$(CC) $(CFLAGS) -c main.cpp -o main.o
	$(CC) $(CFLAGS) *.o -o $(TARGET).out

clean:
	rm -rf *.o *.out

rebuild: all
