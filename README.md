# SimpleOS

## Overview

An 0x86 OS from scratch.

The main goal is to make an operating system that supports 2 NICs and demonstrate NAT.

- [Prerequisites](#Prerequisites)
- [Building](#Building)
- [Running](#Running)

### Prerequisites

- Make sure you have `QEMU` installed.
- Build a cross-compiler for `i386` in `$HOME/opt/cross`.
- Run `source crosscompiler.env`.

### Building

- Build project using `make`.

### Running

- Run `qemu-system-i386 -drive file=os.img,index=0,if=floppy,format=raw` or Build + Run using `make run`.
