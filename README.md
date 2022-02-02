# CPSC 3500: Project 2

> https://seattleu.instructure.com/courses/1601766/assignments/6995263

## About
myshell is a Linux command-line interpreter
It is divided into three files:

myshell.cpp: The main function of the project.
  -Gets the command line input and turns it into a parsed and lexed string vector 
  -It then either creates and runs a single command or runs piped multi-commands.

command.cpp: Responsible for executing the commands and managing pipe file descriptors.

helper.cpp: Contains the function that parses the string vector into tokens, and the function that handles whitespace.

## Team members

- Gary Tou ([@garyhtou](https://github.com/garyhtou))
- Castel Villalobos ([@impropernoun](https://github.com/impropernoun))
- Hank Rudolph ([@hankrud](https://github.com/HankRud))

