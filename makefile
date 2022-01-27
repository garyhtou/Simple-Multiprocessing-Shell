CC=g++
CFLAGS=-Wall -Werror
all: myshell
myshell: myshell.cpp command.cpp
	$(CC) $(CFLAGS) myshell.cpp command.cpp -o myshell