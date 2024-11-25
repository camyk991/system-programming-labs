# Minishell

A simple C program that implements a mini shell supporting piped commands. It parses commands, creates pipelines, and executes them using `execvp`.

## Features
- Supports multiple piped commands separated by a `.`.
- Dynamically creates pipes between commands.
- Handles command execution using child processes.

## Usage
Compile the program:
```bash
gcc -o minishell minishell.c
```
Execute program:
```bash
./minishell <command1> . <command2> . <command3> ...
```
Example:
```bash
./minishell ls -l . sort -u
```

