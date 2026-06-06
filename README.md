# Kyno

Kyno is a minimalist, independent open-source kernel written entirely from scratch in pure C. It operates in a freestanding environment without relying on assembly bootstrap code, external libraries, or standard C dependencies. Built for low-level hardware interaction, it serves as a raw operating system development experiment.

---

## Technical Specifications

* Language: 100% Pure C
* Environment: Freestanding (-ffreestanding)
* Architecture target: x86 / x86_64

---

## How to Compile on Debian

To compile the source file without standard library linking, open your terminal and run:

```bash
gcc -m32 -c Kyno.c -o Kyno.o -ffreestanding -O2 -Wall -Wextra
