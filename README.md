# Minishell

> A robust Unix shell implementation written in C. Developed as part of the 42 curriculum to explore process management, file descriptors, parsing, signal handling, and building custom shell behavior from scratch.

---

## 📑 Table of Contents

- [📚 Project Overview](#project-overview)
- [🧠 Key Concepts](#key-concepts)
- [⚙️ Features](#️features)
- [🛠️ Usage](#usage)
- [📁 Project Structure](#project-structure)
- [🧑‍💻 Author](#author)
- [📜 License](#license)

---

## 📚 Project Overview

**Minishell** is a minimalist yet powerful command-line shell that replicates essential features of Bash. It supports executing programs, pipelines, redirections, built-in commands, and environment variable management.

The goal is to build a reliable, POSIX-compliant shell while handling input parsing, process control, and user interaction — all without relying on external parsing tools.

---

## 🧠 Key Concepts

- Shell command parsing
- Tokenization and abstract syntax tree (AST)
- Built-in command implementation
- Environment variable expansion (`$VAR`)
- Pipes and file descriptor management
- Redirections (`>`, `<`, `>>`, `<<`)
- Forking and process control
- Signal handling (e.g., `Ctrl+C`, `Ctrl+\`)
- Exit codes and error reporting

> 🧩 This project gave us deep insights into Unix internals, shell behavior, and low-level C programming.

---

## ⚙️ Features

- ✅ Execution of binary commands (`ls`, `grep`, `cat`, etc.)
- ✅ Support for built-in commands: `cd`, `echo`, `pwd`, `env`, `export`, `unset`, `exit`
- ✅ Quoting rules: single and double quotes
- ✅ Environment variable expansion
- ✅ I/O redirections: `>`, `>>`, `<`, `<<` (heredoc)
- ✅ Pipeline chaining with `|`
- ✅ Exit status management
- ✅ Robust error messages
- ✅ Signal handling for interactive mode

---

## 🛠️ Usage

1. **Clone the repository And Navigate to it:**

```bash
 git clone https://github.com/Mohamed-ait-alla/minishell.git
 cd minishell
```

2. **Compile the source files:**
```bash
	make
```

3. **running the shell:**
```bash
	./minishell
```

#### Example Of Usage

```bash
minishell> ls -l                       # simple external command
total 11
-rw-r--r-- 1 user groups  2928 May 24 19:08 file


minishell> echo "Hello, $USER"         # built-in command with expansion
Hello, student42


minishell> ls | grep .c | wc -l        # multiple pipes
12


minishell> cat << EOF                  # here-document
> This is heredoc
> EOF
This is heredoc

```

## 📁 Project Structure
```bash
minishell/
📁 execution-part/             # Contains everything related to command execution
│   📁 builtins/               # Implementation of shell built-in commands (cd, echo, etc.)
│   │   📄 builtin_*.c
│   📄 *.c                     # Executor logic, redirections, pipes, process handling
│
📁 includes/                   # Project-wide header file(s)
│   📄 minishell.h
│
📁 libft/                      # Custom library of basic functions (libft)
│   📄 ft_*.c
│   📄 libft.h
│   📄 Makefile
│
📁 parsing-part/               # Everything related to parsing user input
│   📄 *.c                     # Tokenization, expansion, syntax validation
│
📁 tests/                      # Manual and automated tests
│   📄 common_tests.md         # Shell command tests used for verifying behavior
│   📄 filevalgrind.txt        # Used to suppress false positives from `readline` leaks
│   📄 tester.js               # Helper script for testing the parsing logic
│
📄 main.c                      # Entry point of the program that initializes and runs the shell  
📄 Makefile                    # Compiles the source files into the `minishell` binary  
📄 README.md                   # You're reading it :)

 ```

## 🧑‍💻 Author

Mohamed Ait Alla | [GitHub](https://github.com/Mohamed-ait-alla)
<br>
Mohamed Dahani   | [GitHub](https://github.com/dahani-dev)

📜 License

This project is part of the 42 School curriculum and follows educational usage guidelines.