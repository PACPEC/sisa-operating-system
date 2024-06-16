# sisa-operating-system
SOS, an operating system for the SISA-S architecture

## File structure
The files that can be found on the project are divided into user and system execution modes

#### System files
- entry.S
- kernel.c
- kernel\_utils.c and kernel\_utils.h
- syscalls.c and syscalls.h

#### User files
- user.c
- wrappers.S and wrappers.h

#### Support and configuration files
- Makefile
- system.lds
- limpiar.pl
- macros.s
