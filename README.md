# minishell
# Minishell

## Description
Minishell is a simplified implementation of a shell, similar to bash, written in C. This project is focused on creating a functional command-line interface that can interpret and execute commands, handle environment variables, implement redirections and pipes, and include several built-in commands.

## Features

### Basic Shell Functionality
- Interactive command prompt
- Command history navigation
- Command execution using PATH variable or absolute/relative paths
- Signal handling (ctrl-C, ctrl-D, ctrl-\\)

### Environment Variable Management
- Environment variable expansion (`$VAR`)
- Exit status access (`$?`)

### Redirections
- Input redirection (`<`)
- Output redirection (`>`)
- Heredoc (`<<`)
- Append output (`>>`)

### Pipes
- Command pipelines using pipe character (`|`)

### Built-in Commands
- `echo` with `-n` option
- `cd` with relative or absolute path
- `pwd` with no options
- `export` with no options
- `unset` with no options
- `env` with no options or arguments
- `exit` with no options

### Bonus Features (if implemented)
- Logical operators (`&&`, `||`) with parenthesis for priorities
- Wildcard (`*`) expansion for the current working directory

## Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/minishell.git

# Navigate to the project directory
cd minishell

# Compile the project
make

# Run the shell
./minishell
```

## Usage Examples

```bash
# Basic command execution
$ ls -la

# Using redirections
$ cat < input.txt > output.txt

# Using pipes
$ ls -l | grep .c

# Environment variables
$ echo $HOME

# Using built-in commands
$ cd ../
$ pwd
$ echo Hello World
```

## Project Structure
```
.
├── Makefile          # Compilation rules
├── includes/         # Header files
├── libft/            # Custom library functions
└── srcs/             # Source files
    ├── builtins/     # Implementation of built-in commands
    ├── exec/         # Command execution
    ├── lexer/        # Lexical analysis
    ├── parser/       # Command parsing
    └── utils/        # Utility functions
```

## Requirements
- GCC compiler
- GNU Make
- GNU Readline library

## Resources
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)

## License
This project is part of the curriculum at [Your School Name].