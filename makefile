CC=g++
CFLAGS=-Wall -Werror
all: myshell
myshell: myshell.cpp command.cpp helper.cpp
	$(CC) $(CFLAGS) myshell.cpp command.cpp helper.cpp -o myshell
rebuild: clean all
clean:
	rm -f myshell