# Simple In-Memory Filesystem in C

This is a minimal command-line filesystem simulation written in C. It supports basic operations like creating directories and files, navigating between directories, modifying permissions and ownership, and performing basic file access actions.

## Features

- `mkdir <name>` – Create a new directory
- `touch <name>` – Create a new file
- `ls` – List all files and directories in the current directory
- `cd <name>` – Change to a subdirectory
- `cd ..` – Move one directory up
- `rm <name>` – Remove a file or directory (requires write permission)
- `chmod <permission> <name>` – Change permissions (0-7)
- `chown <owner> <name>` – Change file/directory owner
- `read <name>` – Read a file (requires read permission)
- `write <name>` – Write to a file (requires write permission)
- `exec <name>` – Execute a file (requires execute permission)
- `quit` – Exit the program

## Permissions

Permissions follow Unix-style numeric representation:

| Number | Permission |
|--------|------------|
| 0      | ---        |
| 1      | --x        |
| 2      | -w-        |
| 3      | -wx        |
| 4      | r--        |
| 5      | r-x        |
| 6      | rw-        |
| 7      | rwx        |

Only the user who owns the file/directory or the `root` user can change permissions and ownership.

## Structure

- Files and directories are represented using a `struct node` structure.
- The whole filesystem is stored in memory (no disk persistence).
- A single user `root` is initialized with full access.

## Usage

1. Compile the program:
   ```bash
   gcc -o fs main.c
