# Overview of CPU hardware organization with focus on memory
## Terminologies: 

- **Word size**: The transferred chunks of bytes are known as words. The number of bytes in a word is called the word size. e.g. 32-bit system: 4 bytes, 64-bit system: 8 bytes.
- **Bandwidth**: The maximum rate at which byte chunks can be transferred.
- **I/O devices**: Input/Output devices are the connection to the outer world. Examples: keyboard, mouse, display, disk drive, printer, GPU (rendering, PCIe).
- **Controllers/adapters**: Devices that transfer information between I/O bus and I/O device.

## HARDWARE ORGANIZATION
The main components are: Buses, I/O devices, Main memory (temporary storage), Processor (central processing unit)

### Main memory:
- Physically, main memory consists of a collection of dynamic random access memory (DRAM). "Dynamic" means that the memory cells must be refreshed periodically (DRAM has a frequency, usually MHz)  
- Each byte has one address:
32-bit system: 4294967296 addresses → maximum 4GB DRAM
64-bit system: 18446744073709551616 addresses → maximum 16EB DRAM
    
### Processor:
- The processor operates with a (simple) instruction model, defined by its instruction set architecture (ISA). Two common models are RISC (reduced instruction set computer) and CISC (complex instruction set computer).
-  CPU operations revolve around main memory, the register file and the arithmetic/logic unit (ALU)
   -  The register file is a small storage device that consists of a collection of word-sized registers
   -  The ALU computes new data and address values. Some simple operations the CPU might carry out:
        - `Load`: Copy a byte or a word from main memory into a register, overwriting the previous contents of the register
        - `Store`: Copy a byte or a word from a register to an address in main memory, overwriting the previous contents at that base address
        - `Move`: Move the value in one register to another (no access to main memory)
        - `Compare`: Compare the value in a register to zero
        - `Jump`: Extract a word from the instruction itself and copy that word into the program counter (PC), overwriting the previous value of the PC

### What happens when you run a program?
- The high-level code must be translated into a sequence of low-level machine-language instructions
  1. **Preprocessor**: modifies the original program depending on directives that start with "#". Comments will be removed as well. Headers are expanded and replaced with code in this phase.
   Check with ```$ gcc -E programName.c >programName.i```
  2. **Compilation (cc1)**: translates the pre-processed file into an assembly-language file. Assembly language is useful because it allows us to inspect the generated machine instructions in a human readable form.
    Check with ```$ gcc -S programName.c```
  3. **Assembly**: translates assembly code input into machine-language instructions and packages them into a form known as a relocatable object program which usually have a *.o file suffix.
    Check with ```$ gcc -c programName.c```. To disassemble the object file, use ```$ objdump -d programName.o```
    1. **Linking**: links the object file with other necessary object files (e.g. printf.o) to create the executable file. The result is the executable file that can be loaded into memory and executed by the system.

## Von Neumann Architecture
- The classical von Neumann architecture simply consists of main memory, a processor and an interconnect between memory and the processor. Main memory stores both, instructions and data. Transfer of data and instructions goes through the interconnect (bus).
>[!NOTE]
> Bottleneck: ...

### Von Neumann Architecture v/s Today
Classic von Neumann architecture would not work today because processors transform data at a rate higher than the data can be fed into the processor → caches!

## Memory
- The memory pyramid
- Virtual memory
- Virtual Address Space (Linux Process)

main memory (DRAM) is slow! SRAM is fast but more expensive
Each level serves as a cache for the next lower level

## What are the bottlenecks
## Memory
## The memory pyramid
## Virtual memory Linux process anatomy
Discussion of reading assignment: There's plenty of room at the top
Brief introduction to computing resources (lab1)
  


  WHAT HAPPENS WHEN YOU RUN A PROGRAM?
Assume we have this simple high-level C program
   1 #include <stdio.h> 2
   int main(void)
   {
       printf("hello, world!\n");
       return 0;
3546
7}
(high-level because humans can read and understand it)
In order to run this program on the system, the high-level code must be translated into a sequence of low-level machine-language instructions
A compiler translates the source code into machine instructions which are packaged int an executable object program (or binary). For the code above we would execute
$ gcc -o hello hello.c
What happens when we call gcc:
     o

  WHAT HAPPENS WHEN YOU RUN A PROGRAM?
Preprocessing phase:
The preprocessor modifies the original program depending on directives that start with "#". Comments will be removed as well. For example, the
#include <stdio.h> header in our code would be expanded and replaced with code in this phase. You can investigate this stage with
     $ gcc -E hello.c >hello.i
   
  WHAT HAPPENS WHEN YOU RUN A PROGRAM?
  Compilation phase:
The compiler translates the pre-processed file into an assembly-language file. Assembly language is useful because it allows us to inspect the generated machine instructions in a human readable form. This is important when we optimize. Moreover, it provides a common output language, i.e. C/C++ and Fortran would look the same on this level.
  Example assembly code for the hello.c program. Lines 2-7 are each machines instructions. We will get back to assembly a bit later.

  WHAT HAPPENS WHEN YOU RUN A PROGRAM?
Assembly phase:
The assembler translates assembly code input into machine-language instructions and packages them into a form known as a relocatable object program which usually have a *.o file suffix. This output can be generated with
     $ gcc -c hello.c
When we disassemble hello.o we find that our main function is encoded by 23 bytes and we have a much harder time to figure out what this machine code does (without the assembly on the right). We do not need to understand this output for CS205!
      $ objdump -d hello.o
0000000000000000 <main>:
0: 48 83 4: 48 8d b: e8 00
10: 31 c0 12: 48 83 16: c3
ec 08
3d 00 00 00 00 00 00 00
c4 08
sub $0x8,%rsp
lea 0x0(%rip),%rdi call 10 <main+0x10> xor %eax,%eax
add $0x8,%rsp
ret
# b <main+0xb>
 
  WHAT HAPPENS WHEN YOU RUN A PROGRAM?
Linking phase:
Note that our program calls printf which is part of the standard C library provided by the compiler (similar to the OpenMP library that we will use later in the class). The
printf function resides in a separate printf.o file which must be linked to our hello.o file. The result is the executable hello file that can be loaded into memory and executed by the system. Assuming the printf.o file was in the same directory as hello.o we could run:
     $ gcc -o hello hello.o printf.o
(In practice printf.o is part of the standard library and the compiler will find it without us explicitly specifying it.)
     
  WHAT HAPPENS WHEN YOU RUN A PROGRAM?
Given the hardware organization we discussed, this is what happens when we execute
We type the ./hello characters in the shell which are read one by one into a register and stored in main memory. When we hit enter theshellknowswearedoneandexecutionoftheprogram begins.
The shell then loads the executable hello file by executing the instructions that copy the code and data in the hello object file from disk to main memory. (Note: this happens without involving the processor due to direct memory access (DMA).) The data includes the string "hello, world!\n" which is encoded in the executable.
Once the code and data in the hello object file are loaded in memory, the processor begins to execute the instructions in the main function. These instructions copy the hello, world!\n string (characters) from memory to the register file and from there to the display device.
   $ ./hello
hello, world!
          
  VON NEUMANN ARCHITECTURE
Let us neglect the I/O devices we have seen in the previous slides. The image below corresponds to the classical von Neumann architecture:
    CPU
 Register file
    ALU
   Bus interface
   System bus
Memory bus
   I/O bridge
Main memory
  Classic von Neumann architecture
John von Neumann
The classical von Neumann architecture simply consists of main memory, a processor
and an interconnect between memory and the processor. Main memory stores both, instructions and data. Transfer of data and instructions goes through the interconnect (bus)
What could be a potential problem with this architecture? (especially today)
   
  VON NEUMANN ARCHITECTURE
von Neumann bottleneck: separation of processor and memory Factory/warehouse example:
Warehouse (main memory) Traffic (interconnect/bus) Factory (processor)
The interconnect determines the rate at which instructions and data can be accessed. The traffic jam above is problematic because the folks in the factory will have to wait.
Latency: the response time for information to arrive measured from its initial request. Latency is often related to physical parameters
the time to fill the pipe is proportional to its dimension.
Bandwidth: the throughput of a subsystem. Naively the inverse of latency (neglecting concurrency). Modern subsystems can overlap multiple requests to increase bandwidth! bandwidth is more generally defined as the rate at which requests can be satisfied.
          
  VON NEUMANN ARCHITECTURE
Example: is a CPU starving for data?
We want to assess how relevant the von Neumann bottleneck is today
Recent 12th Gen Intel Core i9-12900E:
About 1 Tflop/s single precision floating point peak performance
About 80 GB/s memory bandwidth to DRAM
How many more single precision floating point numbers can this chip process compared to the amount of floating point numbers that can be delivered to the ALU of the chip at any given time?
     
  VON NEUMANN ARCHITECTURE VS. TODAY
• CPUs of today would never be this performant if it was based on the classic von Neumann architecture. The gap between performance and memory bandwidth keeps increasing. It is easier to improve CPU performance than memory performance.
• What do you think is the main reason for GPUs being much more expensive than CPUs?
There are three main modifications in recent CPUs:
Caches
To reduce the processor-memory gap, smaller and faster cache memories (or just caches) are used which are usually on the CPU chip. Most often there are three levels: L1 (closest to ALU, smallest size), L2 and L3 (furthest from ALU, largest size). The idea of caching is to exploit locality. It is a very important concept for HPC and we will look at caches in the next lecture.
Virtual memory
A similar caching problem exists for DRAM and secondary storage systems. Virtual memory further helps the operating system manage multiple processes that run simultaneously and thus request a fraction of main memory. (Virtual memory is less important for CS205 and we will not look into it in detail.)
Low-level parallelism
This mainly includes instruction-level parallelism (ILP) on the hardware level. This concept entails pipelining and multiple issue of instructions (lecture 16). Coarse grained parallelism is exploited through thread-level parallelism which we will be concerned with in the shared memory part of the class.
 
  MEMORY PYRAMID
      Smaller, faster, and costlier (per byte) storage devices
Larger, slower, and cheaper
(per byte) L5: storage
devices
L6:
L1:
L0: Regs
L1 cache (SRAM)
L2 cache (SRAM)
L3 cache (SRAM)
Main memory (DRAM)
CPU registers hold words retrieved from cache memory.
L1 cache holds cache lines retrieved from L2 cache.
L2 cache holds cache lines retrieved from L3 cache.
L3 cache holds cache lines retrieved from memory.
Main memory holds disk blocks retrieved from local disks.
Local disks hold files retrieved from disks on remote network server.
       L2:
L3:
            L4:
     Local secondary storage (local disks)
     Remote secondary storage (distributed file systems, Web servers)
 • Since 1985, the cost per megabyte DRAM has decreased by a factor of 44'000(!), its access time has improved only by a factor of 10
• The cost of SRAM is about 50-100 times higher than DRAM because of the manufacturing process as well as one SRAM cells requires 6 transistors while DRAM requires 1 (and a capacitor). SRAM is much more power hungry than DRAM.

  LATENCY FIGURES YOU SHOULD BE AWARE OF
You should be aware of the following characteristic latencies:
     Type
CPU registers L1 cache
L2 cache
L3 cache Virtual memory Disk cache Browser cache Web cache
Where cached
On-chip CPU registers On-chip L1 cache On-chip L2 cache On-chip L3 cache Main memory (DRAM) Disk controller
Local disk
Remote server disks
Latency (cycles)
0
4 10 50 200 100'000 10'000'000 1'000'000'000
Managed by
Compiler Hardware Hardware Hardware Hardware + OS Controller firmware Web browser
Web proxy server
          Do you have a feel for the difference of latency between a L1 cache and main memory?

  VIRTUAL MEMORY
• Virtual memory is an abstraction that provides each process with the illusion that it has exclusive use of the main memory
• Each process "sees" the same uniform view of memory which is known as its virtual address space
• In reality, there is one large pool of physical DRAM memory with 4294967296 unique addresses on 32-bit systems or 18446744073709551616 unique addresses on a 64-bit system
• The operating system manages many processes which may run concurrently on the system. Virtual memory protects the address space of each process from corruption by other processes
• Under the hood, a lookup of a virtual memory address (requested from a process) must be translated to a physical memory address. Dedicated hardware on the CPU chip called memory management unit (MMU) translates virtual addresses on the fly
 
  VIRTUAL MEMORY
Under the hood, a lookup of a virtual memory address (requested from a process) must be translated to a physical memory address. Dedicated hardware on the CPU chip called memory management unit (MMU) translates virtual addresses on the fly
Virtual address (VA) to physical address (PA) translation through MMU when loading a 4 byte word
                                                                  
  VIRTUAL ADDRESS SPACE (LINUX PROCESS)
  High address
 1 #include <stdio.h> 2
3 int main(void) 4{
5 printf("hello, world!\n");
6 return 0;
7}
                   Program start Low address
 
    High address
VIRTUAL ADDRESS SPACE (LINUX PROCESS)
Program code and data
Code begins at the same fixed address for all processes, followed by data locations that correspond to global C/C++ variables. The code and data areas are initialized directly from the contents of an executable object file, here the hello executable.
 1 #include <stdio.h> 2
3 int main(void) 4{
5 printf("hello, world!\n");
6 return 0;
7}
                     Program start
  Low address

    High address
VIRTUAL ADDRESS SPACE (LINUX PROCESS)
Heap
The code and data areas are followed immediately by the run- time heap. Unlike the code and data areas (which are fixed in size once the process begins running) the heap expands and contracts dynamically at run time as a result of calls to malloc or free for example. These calls involve the OS kernel which are therefore slow.
 1 #include <stdio.h> 2
3 int main(void) 4{
5 printf("hello, world!\n");
6 return 0;
7}
                     Program start
  Low address

    High address
VIRTUAL ADDRESS SPACE (LINUX PROCESS)
Shared libraries
Near the middle of the address space is an area that holds code and data for shared libraries such as the C/C++ standard libraries and the math library for example.
 1 #include <stdio.h> 2
3 int main(void) 4{
5 printf("hello, world!\n");
6 return 0;
7}
                     Program start
  Low address

    High address
VIRTUAL ADDRESS SPACE (LINUX PROCESS)
Stack
At the top of the user's virtual address space is the user stack that the compiler uses to implement function calls. The user stack expands and contracts dynamically during the execution of the program. When we call a function, the stack grows downward and when we exit a function it contracts upward. Unlike the heap, memory requests on the stack are cheap.
 1 #include <stdio.h> 2
3 int main(void) 4{
5 printf("hello, world!\n");
6 return 0;
7}
                    Program start
  Low address

    High address
VIRTUAL ADDRESS SPACE (LINUX PROCESS)
Kernel virtual memory
The top region of the address space is reserved for the kernel. Application programs are not allowed to read or write the contents of this area or to directly call functions defined in the kernel code. Instead, they must invoke the kernel to perform these operations.
 1 #include <stdio.h> 2
3 int main(void) 4{
5 printf("hello, world!\n");
6 return 0;
7}
                    Program start
  Low address

  READING: THERE'S PLENTY OF ROOM AT THE TOP
In the first column of the summary, the authors mention:
   Unfortunately, semiconductor miniaturization is running out of steam as a viable way to grow computer performance—there isn't much more room at the "Bottom."
What do they mean by that?

  READING: THERE'S PLENTY OF ROOM AT THE TOP
 How are these two figures related?
Cameron et al., IEEE 2005
Leiserson et al., Science 2020
                                                       
  READING: THERE'S PLENTY OF ROOM AT THE TOP
What is the message of the data presented in this table? From a software engineering perspective, is it always a good idea to go all this way?
                                                                    • What is the parallelization the authors used in Version 4?
Leiserson et al., Science 2020
• For Version 5 the authors mention that "spatial and temporal locality" has been exploited. Which part of the hardware did they target for this optimization (in hindsight of the lecture today)? How dramatic is the relative speedup?
• Version 6 and 7 deal with vectorization. What is meant by that? What is "AVX" and will you have this on any CPU? Recall Flynn's taxonomy: to which classification does Version 6 and 7 belong? (SIMD, MISD or MIMD)

  INTRO TO COMPUTING RESOURCES
You will need some computing resources to solve the homeworks and tasks in the labs
Several options:
You can work locally on your laptop to develop a solution to the problem and test on
the cluster when done. MPI can be installed easily on MacOSX or Linux.
We have a docker image with the tools for the class (likely useful for Windows users or
WSL). It is provided “as is”.
$ docker pull iacs/cs205_ubuntu
We have access to the Harvard academic cluster
(Linux system, SLURM job scheduler → lab 1)
The academic cluster is the reference platform. If you are asked to benchmark a problem or report performance results, you are expected to produce these results on the academic cluster.
Results in solutions are obtained from the Harvard academic cluster.

