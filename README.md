# alloc

## Overview

**alloc** is a simple allocator written in C. It posseses the following characteristics:

- Written in C99;
- It is an implicit free list memory allocator;
- Check the [main.c](./main.c) file for an usage example;
- This code is platform dependent and will work only on Linux;
- Uses the [mmap](https://man7.org/linux/man-pages/man2/mmap.2.html) syscall;
- It is not recomended to use this allocator in practice. This was only an learning exercise. Insted, use the [malloc/free](https://man7.org/linux/man-pages/man3/free.3.html) functions;

## Usage

Download this project and compile it by typing the command `make` in its folder. Next, just run the executable `./alloc`. Like this:

```
make
./alloc
```