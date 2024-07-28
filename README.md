# MiniShell

## Table of Contents
- [Introduction](#introduction)

## Introduction
Welcome to MiniShell, a lightweight and simple Unix shell implementation developed as part of the 42 School curriculum. This project is designed to provide a deep understanding of how a shell works by replicating the core functionalities of popular Unix shells like bash or zsh.

The MiniShell project challenges students to delve into various aspects of system programming, process control, and command execution. By building MiniShell, you will gain hands-on experience with concepts such as:

- **Process Management**: Learn how to create, manage, and synchronize multiple processes using system calls like `fork`, `exec`, and `wait`.
- **File Descriptors and Redirection**: Understand how to manipulate file descriptors to redirect input, output, and errors, enabling functionalities like piping and I/O redirection.
- **Environment Variables**: Explore how environment variables are managed and utilized within a shell environment.
- **Signal Handling**: Implement robust signal handling to manage interrupts and termination signals, ensuring your shell can handle common scenarios like `Ctrl+C` and `Ctrl+D`.

MiniShell aims to provide a solid foundation for understanding the inner workings of a Unix shell, preparing you for more advanced topics in system programming and software development. We hope you enjoy the journey of creating your very own shell!

## Features
- Command execution with arguments
- Redirection of input and output
- Piping between commands
- Environment variable handling
- Signal handling (e.g., `Ctrl+C`, `Ctrl+D`)
- Built-in commands:
  - `cd`
  - `echo`
  - `env`
  - `exit`
  - `export`
  - `pwd`
  - `unset`
