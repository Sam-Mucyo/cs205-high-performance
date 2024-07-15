### str_mutability.c

On Mac M1, the code raised a `SIGBUS` error, while on AMD64-Linux Docker environment, it resulted in
a `SIGSEGV` error. Why is that?

```c
char *s_ptr = "Hello, World!";
.
.
s_ptr[0] = 'k';
```

`s_ptr` points to the string literal "Hello, World!", which is stored in a read-only section of the program's memory.
Trying `s_ptr[0] with s_ptr[0] = 'k';` attempts to change the contents of a string literal, which is not allowed and leads to a runtime error (bus error on your Mac M1 and segmentation fault on AMD64-Linux).

The takeaway modifying a string literal results in an undefined behavior. For portable & correct C code: 

1. Treat string literals as read-only.
2. If you need a modifiable string, use an array as in `s_arr`.
3. Be aware that pointers to string literals (`char *s = "hello"`) point to immutable data.

See  Also [Chapter 6 of ptrtut1.2](https://sites.cs.ucsb.edu/~mikec/cs16/misc/ptrtut12/ch6x.htm) for general concepts.
For more on the specifics of `SIGSEGV` and `SIGBUS`, see [SIGSEGV vs SIGBUS](https://www.geeksforgeeks.org/segmentation-fault-sigsegv-vs-bus-error-sigbus/).
