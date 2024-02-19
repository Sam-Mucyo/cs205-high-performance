# Lecture 5: Memory consistency model, critical sections and mutual exclusion

## Memory Consistency Models

**Memory Consistency Model**: Dictates the order in which read and write operations are executed across threads, ensuring that parallel programs produce correct and predictable results.

**Sequential Consistency**: A straightforward model where operations by all threads appear in a specific sequential order. While intuitive, it restricts compiler optimizations. 

**Relaxed Consistency Models**: Allow for out-of-order execution of read and write operations for performance gains, requiring explicit synchronization to maintain program correctness.

## Synchronization Primitives

**Synchronization Primitives**: Tools used to control the execution order of read and write operations in parallel code, crucial for avoiding race conditions and ensuring correctness.
- Examples include the `#pragma omp atomic` directive in OpenMP and `std::atomic types in C++ to enforce atomic operations.

## Relaxed consistency Model
- What is a relaxed consistency model?


## Mutual exclusion / critical sections / locks
- A **lock**/**mutex** is an abstract data type that enforces mutual exclusion for a critical section.

### Attempts for implementing a lock
- What is a spin lock?
  > A spin lock is a lock that spins until it can acquire the lock. This is a busy waiting lock.

## Overview of thread libraries



# Relevant CS61 Notes
Adapted from [Synchronization](https://cs61.seas.harvard.edu/site/2023/Synch/#gsc.tab=0)

## Threads
A program can define one process that contains multiple threads. All threads within the same process share the same virtual address space and file descriptor table. However, each thread will have its own set of registers and its own stack. [A thread's stack does live in memory shared with other threads, so the stack can be corrupted by a memory bug in the code running in another thread!]

In general, the kernel will need to store a set of registers for each thread in a process.

Let's look at how to use threads using our example program in `incr-basic.cc`: #TODO:

```c++
void threadfunc(unsigned* x) {
    // This is a correct way to increment a shared variable!
    // ... OR IS IT?!?!?!?!?!?!??!?!
    for (int i = 0; i != 10000000; ++i) {
        *x += 1;
    }
}

int main() {
    std::thread th[4];
    unsigned n = 0;
    for (int i = 0; i != 4; ++i) {
        th[i] = std::thread(threadfunc, &n);
    }
    for (int i = 0; i != 4; ++i) {
        th[i].join();
    }
    printf("%u\n", n);
}
```

In this code, we run the function threadfunc() in parallel in 4 threads. The std::thread::join() function makes the main thread block until the thread upon which the join() is called finishes execution. So, the final value of n will be printed by the main thread after all 4 threads finish.

In each thread we increment a shared variable 10 million times. There are 4 threads incrementing in total and the variable starts at zero. What should the final value of the variable be after all incrementing threads finish? 40 million seems like a reasonable answer, but by running the program we observe that sometimes it prints out values like 30 million or 20 million. What's going on?

First of all, note that an optimizing compiler will optimize away the loop in threadfunc(). In particular, the compiler will recognize that the loop is simply incrementing the shared variable by an aggregate of 10 million; so, the compiler will transform the loop into a single addl $10000000, (%rdi) instruction with the constant value 10 million.

However, there's a second problem—a race condition in the addl instruction itself! Up until this point in the course, we've been thinking x86 instructions as being indivisible and atomic. In fact, they are not, and their lack of atomicity shows up in a multi-processor environment.

Inside the CPU hardware, the addl $10000000, (%rdi) is actually implemented as 3 separate "micro-op" instructions:

movl (%rdi), %temp (load)
addl $10000000, %temp (add)
movl %temp, (%rdi) (store)
Imagine 2 threads executing this addl instruction at the same time (concurrently). Each thread loads the same value of (%rdi) from memory, then adds 10 million to it in their own separate temporary registers, and then write the same value back to (%rdi) in memory. The last write to memory by each thread will overwrite each other with the same value, and one increment by 10 million will essentially be lost.

This is the behavior of running 2 increments on the same variable in x86 assembly. In the C/C++ abstract machine, accessing shared memory from different threads without proper synchronization is undefined behavior, unless all accesses are reads.

Re-running this experiment with compiler optimizations turned off (so the loop does not get optimized away), we will get a result like 15285711, where roughly 2/3 of the updates are lost.

There are 2 ways to synchronize shared memory accesses in C++. We will describe a low-level approach, using C++'s std::atomic library, and then introduce a higher-level, more general way of performing synchronization.

## Data races

The foundation of correct synchronization is a law we call



[Atomic objects are defined below.]

For example, our incr-basic.cc function violates the Fundamental Law because multiple threads concurrently write the n object.

The undefined behavior that occurs when two or more threads have conflicting concurrent accesses to an object is called a data race. Data races are bad news. But note that any number of threads can concurrently and safely read an object; a data race only occurs when at least one of the accesses is a write.

Atomics
incr-atomic.cc implements synchronized shared-memory access, and avoids data races, using C++ atomics. The relevant code in threadfunc() is shown below.

void threadfunc(std::atomic<unsigned>* x) {
    for (int i = 0; i != 10000000; ++i) {
        *x += 1;   // compiles to atomic “read-modify-write” instruction
        // `x->fetch_add(1)` and `(*x)++` also work!
    }
}
C++'s atomics library implements atomic additions using an x86's atomic instruction. When we use objdump to inspect the assembly of threadfunc(), we see an lock addl ... instruction instead of just addl .... The lock prefix of the addl instruction asks the processor to hold onto the cache line with the shared variable (or in Intel terms, "lock the memory bus") until the entire addl instruction has completed.

Because the compiler understands how to implement the atomics library, atomic accesses never cause data races. Specifically, multiple threads can read and write atomic objects concurrently without causing undefined behavior—atomics never violate the Fundamental Law of Synchronization.

Of course, that doesn’t mean that all programs which use atomics are correct. Consider this threadfunc:

void threadfunc(std::atomic<unsigned>* x) {
    for (int i = 0; i != 10000000; ++i) {
        unsigned v = *x;  // read
        v = v + 1;        // modify
        *x = v;           // write
    }
}
Because this function separates the read, modify, and write steps into separate operations, an incr program using the function will almost certainly produce the wrong answer. Two threads can explicitly read the same value from *x, which will lose an increment. But there is no undefined behavior!

Synchronization objects
C++ atomics and lock-prefixed instructions only work with aligned objects with sizes of one word (8 bytes) or less. This is great, but limited. To synchronize more complex objects in memory or perform more complex synchronization tasks, we need abstractions.

Synchronization objects are types whose methods can be used to achieve synchronization and atomicity on normal (non-std::atomic-wrapped) objects. Synchronization objects provides various abstract properties that simplify programming multi-threaded access to shared data. The most common synchronization object is called a mutex, which provides the mutual exclusion property.

## Mutual exclusion
Mutual exclusion means that at most one thread accesses shared data at a time.

Our multi-threaded `incr-poll.cc` example does not work because more than one thread can access the shared variable at a time. The code would behave correctly if the mutual exclusion policy is enforced. We can use a mutex object to enforce mutual exclusion (incr-mutex.cc). In this example it has the same effect as wrapping *x in a std::atomic template:

std::mutex mutex;

void threadfunc(unsigned* x) {
    for (int i = 0; i != 10000000; ++i) {
        mutex.lock();
        ++*x;
        mutex.unlock();
    }
}
The mutex has an internal state (denoted by state), which can be either locked or unlocked. The semantics of a mutex object is as follows:

Upon initialization, state = unlocked.
mutex::lock() method: waits until state becomes unlocked, and then atomically sets state = locked. Note the two steps shall complete in one atomic operation.
mutex::unlock() method: asserts that state == locked, then sets state = unlocked.
The mutual exclusion policy is enforced in the code region between the lock() and unlock() invocations. We call this region the critical section.

Implementing a mutex
Let's now think about how we might implement such a mutex object.

Does the following work?

struct mutex {
    static constexpr int unlocked = 0;
    static constexpr int locked = 1;

    int state = unlocked;

    void lock() {
        while (state == locked) {
        }
        state = locked;
    }

    void unlock() {
        state = unlocked;
    }
};
No! Imagine that two threads calls lock() on an unlocked mutex at the same time. They both observe that state == unlocked, skip the while loop, and set state = locked, and then return. Now both threads think they have the lock and proceeds to the critical region, all at the same time! Not good!

In order to properly implement a mutex we need atomic read-modify-write operation. std::atomic provides these operations in the form of the operator++ and operator-- operators. The following mutex implementation is correct:

struct mutex {
    std::atomic<int> state = 0;

    void lock() {
        while (++state != 1) {
            --state;
        }
    }

    unlock() {
        --state;
    }
};
Note that the unlock() method performs an atomic decrement operation, instead of simply writing 0. Simply storing 0 to the mutex state is incorrect because if this store occurred between the ++state and --state steps in the while loop in the lock() method, state becomes negative and the while loop can never exit.

Implementing a mutex with a single bit
We just showed that we can implement a busy-waiting mutex using an atomic counter. But a mutex can also be implemented using just a single bit, with some special atomic machine instructions!

First, consider this busy-waiting mutex (also called a spin lock):

struct mutex {
    std::atomic<int> spinlock = 0;

    void lock() {
        while (spinlock.swap(1) == 1) {
        }
    }

    void unlock() {
        spinlock.store(0);
    }
};
The spinlock.swap() method is an atomic swap method, which in one atomic step stores the specified value to the atomic spinlock variable and returns the old value of the variable.

It works because lock() will not return unless spinlock previously contains value 0 (which means unlocked). In that case it will atomically stores value 1 (which means locked) to spinlock; this prevents other lock() calls from returning, ensuring mutual exclusion. While lock() spin-waits, it simply swaps spinlock's old value 1 with 1, effectively leaving the lock untouched. Please take a moment to appreciate how this simple construct correctly implements mutual exclusion.

x86-64 provides this atomic swap functionality via the lock xchg assembly instruction. We have shown that it is all we need to implement a mutex with just one bit. x86-64 provides a more powerful atomic instruction that further opens the possibility of what can be done atomically in modern processors. The instruction is called a compare-exchange, or lock cmpxchg. It is powerful enough to implement atomic swap, add, subtract, multiplication, square root, and many other things you can think of.

The behavior of the instruction is defined as follows:

// Everything in one atomic step
int compare_exchange(int* object, int expected, int desired) {
    if (*object == expected) {
        *object = desired;
        return expected;
    } else {
        return *object;
    }
}
This instruction is also accessible as the this->compare_exchange_strong() member method for C++ std::atomic type objects. Instead of returning an old value, it returns a boolean indicating whether the exchange was successful.

For example, we can use the compare-exchange instruction to atomically add 7 to any integer:

void add7(int* x) {
    int expected = *x;
    while (compare_exchange(x, expected, expected + 7)
            != expected) {
        expected = *x;
    }
}
