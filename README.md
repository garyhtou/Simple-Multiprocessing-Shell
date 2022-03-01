# 🐚 [**myshell**] A Simple Shell with Multiprocessing and Pipes

## About

`myshell` is a simple shell (command-line interpreter) capable of handling one
command (with or without pipes).

It reads a single line from the `stdin`, lexes it, then executes it via
`execvp`.

<sub>More information can be found [here](/assignment/Project2_shell.pdf).</sub>

## Team members and contribution

- Gary Tou ([@garyhtou](https://github.com/garyhtou))
  - Parsing and lexing function (`Helper::lex`) that also handles quoted strings
  - File descriptors and forking
  - Object orientated `Command` class for more organized code
- Castel Villalobos ([@impropernoun](https://github.com/impropernoun))
  - Makefile
  - Handle difference between single vs multi-command
  - Set up pipes
- Hank Rudolph ([@hankrud](https://github.com/HankRud))
  - Setup and call `execvp`
  - Convert C++ string to c string
  - Prompt and get input

### Strengths and Weaknesses

**Strength:**

- Handles quoted strings. eg. `echo "it works on my machine"`
- Object orientated design (See `Command` class)
- Able to handle "unlimited" commands and arguments (not restricted to the 10
  commands and 20 arguments per command requirement from the instruction)
- Well commented

**Weaknesses:**

- Shell does not run continuously
- Does not handle changing operators such as `&&`, `||`, and `;` (it does handle
  `|`)
- Does not handle environment variables

### The program is composed of three files:

- `myshell.cpp`: Contains the main function and handles the coordination of
  multiple commands and pipes.
  - Gets the inputted command and parses and lexes it.
  - Then runs it as either a single command (no pipes) or multi-command (with
    pipes).
- `command.cpp`: Responsible for executing the commands and managing pipe file
  descriptors.
- `helper.cpp`: Contains functions that parses and lexes commands into tokens,
  as well as handles whitespace.
