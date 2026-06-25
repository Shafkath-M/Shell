# Shell

A simple Unix-like shell implemented in C using GNU Readline.

Build
-----
- Requirements: `make`, `gcc`, and the `readline` development headers (e.g. `libreadline-dev`).
- To build: `make`

Run
---
- Start the shell: `./shell`
- Type `exit` to quit.

Features
--------
- Execute external commands
- `cd` builtin
- Output redirection with `>`
- Single pipe support between two commands (e.g. `ls | grep txt`)

Cleanup and backup files
------------------------
- Use `make clean` to remove the `shell` executable.
- Backup/autosave files sometimes present in this folder:
	- `#shell.c#`
	- `shell.c~`
	Remove them with: `rm '#shell.c#' shell.c~`

Contributing
------------
- Suggestions, bug reports, and pull requests are welcome.
