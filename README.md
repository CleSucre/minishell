# Minishell for 42

## What is it?

Minishell is a project for the 42 school.

The goal is to create a simple shell that can run commands, handle signals, and manage environment variables.

## Subject file

The subject file is available [here](resources%2Fen.subject.pdf)

## How to use it?

#### Build and Run

```shell
make run
```

#### Run in debug mode

```shell
make debug
```

Debug mode will force relink and run the program with `valgrind` with the following options:
- --leak-check=full
- --show-leak-kinds=all
- --track-origins=yes

Our minishell also has its own debug mode that enables debug messages.

Exemple:
```shell
minishell has been created successfully.
==377405== Memcheck, a memory error detector
==377405== Copyright (C) 2002-2017, and GNU GPLd, by Julian Seward et al.
==377405== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==377405== Command: ./minishell
==377405==

[DEBUG] History loaded from file
minishell$ ls
[DEBUG] Command ls executed
[DEBUG] Command ls added to history
[DEBUG] Command ls added to history file
minishell$ ls
[DEBUG] Command ls executed
[DEBUG] Command ls not added to history (already the last command)
[DEBUG] Command ls added to history file
minishell$ mkdir test
[DEBUG] Command mkdir test executed
[DEBUG] Command mkdir test added to history
[DEBUG] Command mkdir test added to history file
minishell$
```

## Todo

- [ ] Add perror to error handling
- [ ] Study termios
- [ ] Study get_cursor_position
