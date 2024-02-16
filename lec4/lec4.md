# Introduction to shared memory

When cores cooperate they need to share and/or exchange
information (i.e. data). Exchange of information is done by sharing a memory
address space where multiple cores have read and write privileges.

## Extension to multicore architectures

● CPU architecture level: pipelining, issuing multiple instructions and vector registers (third
extension to von Neumann architecture, does not directly address the bottleneck but
makes processor faster by exploiting low-level parallelism)

## Kinds of parallelism and how the hardware exploits them

### Task-Level Parallelism (TLP)

- This form of parallelism identifies tasks that can be executed in parallel. Shared memory
programming belongs mainly here.

### Data-Level Parallelism (DLP)

- This form of parallelism arises because there are
many data items that can be operated on at the
same time. Vectorization and GPUs belong here.

```c++
int a[1024];
int main(void)
{
    // task 1: set first half of the array to 1
    for (int i = 0; i < 512; ++i) {
        a[i] = 1;
    }
    // task 2: set second half of the array to 2
    for (int i = 512; i < 1024; ++i) {
        a[i] = 2;
    }
    return 0;
}
```

## Differences between processes and threads

Axis | Processes                           |   Threads
--| -----------------------------------|-----------------------------------
Def | An execution sequence within the operating system | An execution sequence within a process
Ownership |Owns a virtual address space.  | Owns their own state and some private memory on the stack
Sharing |No process can access other processes' virtual memory, only through inter-process communication: signals, files, pipes, sockets or shared memory | Shares the application data in main memory with other threads

Hardware threads: Intel Hyper-Threading technology (HT), AMD SMT. They split one physical core into two logical cores. Software threads are scheduled for execution on such logical cores. **Only one can run at a time.**

## Careful sharing: race conditions

1. Why does simply adding a ```#pragma omp parallel for``` to the loop in the Counter (in the /code/counter/counter.cpp) class not work and yield inconsistent and wrong results?

> **Data Race**: variable `count_` is in shared memory and if multiple threads write to this variable we encounter inconsistent and wrong results. We write to `count_` in the `increment()` method which is not ***thread-safe***.

>[!NOTE]
> *The Fundamental Law of Synchronization*: If two or more threads concurrently access a non-atomic object in memory, then all such accesses must be reads. Otherwise, the program invokes undefined behavior.

2. How to fix?
> **Atomic Operation**: An atomic operation is an operation that will always be executed without any other thread being able to read or change its state during the operation. With OpenMP, we can use the `#pragma omp atomic` directive to make the `increment()` method thread-safe.

## Creation of threads: fork/join model

```c++
#pragma omp parallel
{
    /* block of some parallizable code */
}
```

- The primary thread is the main thread that enters the main function in your program (`thread_id = 0`). The `#pragma omp parallel` directive spawns a team of threads that execute the block of code in parallel. At the end of the block, the threads are joined together again and the primary thread continues sequential execution.