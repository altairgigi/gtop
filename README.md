# GTOP

Gtop (Gigi's top) is a simple system monitor made in C (c99/c17).

## Installation

Download the 'gtop' file on your Linux system.

You can 'install' it by putting it in the /bin directory and setting system permissions accordingly.

Call the 'gtop' command from the terminal/shell of your choice.

## Purpose
Gtop is a project aimed at exploring more advanced C instructions and elements while maintaining the code DRY and compact and ensuring readability. 

Gtop includes/ensure: 
* Basic instructions and practices (control flow, structures, pointers, abstraction, polymorphism, etc.) 
* Error handling;
* Directory & file parsing;
* Memory management;
* Dynamic array;
* Function pointer;
* Array sorting (through qsort());
* Basic ANSI interface and I/O interaction;
* More advanced libraries (ctype.h, dirent.h, termios.h etc).

## Features
Gtop is intended to be a simple top-like implementation with a user-chosen sorted visualisation.

Gtop can:
* visualise current uptime and ram and cpu loads;
* visualise tasks currently running and their pid, cpu load and ram use;
* allow user to choose visualisation order: by PID, cpu load or ram use (default: cpu load);
* allow user to terminate program organically (press 'q').

## License

[GPL3.0](https://choosealicense.com/licenses/gpl-3.0/)