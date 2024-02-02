# Introduction to parallel computing
## Basic Terminology
- `TFlops`: TeraFlops, 10^12 floating point operations per second.
- `The Power Wall`: the power consumption of a chip is limited by the amount of heat it can dissipate.
- `Node/Cluster`: A standalone "computer in a box." Usually comprised of multiple CPUs/processors/cores, memory, network interfaces, etc. Nodes are networked together to comprise a supercomputer or cluster.
- `Socket`: The physical location on a motherboard where a CPU is mounted. Usually there is one but there can be multiple. Access to memory modules is routed through sockets.
- `Pipelining`: Breaking a task into steps performed by different processor units, with inputs streaming through, much like an assembly line; a type of parallel computing.
- `Shared memory`: Describes a computer architecture where all processors have direct (usually bus based) access to common physical memory. In a programming sense, it describes a model where parallel tasks all have the same "picture" of memory and can directly address and access the same logical memory locations regardless of where the physical memory actually exists.
- `Symmetric Multi-Processor (SMP)`: Shared memory hardware architecture where multiple processors share a single address space and have equal access to all resources - memory, disk, etc.
- `Distributed memory`: In hardware, refers to network based memory access for physical memory that is not common. As a programming model, tasks can only logically "see" local machine memory and must use communication to access memory on other machines where other tasks are executing.
- `Communication`: Parallel tasks typically need to exchange data. There are several ways this can be accomplished, such as through a shared memory bus or over a network. The actual event of data exchange is commonly referred to as communication regardless of the method employed.
- `Synchronization`: The coordination of parallel tasks in real time, very often associated with communication. Synchronization usually involves waiting by at least one task, and can therefore cause a parallel application's wall clock execution time to increase. Synchronization points serialize a parallel application.

- `Granularity`: In parallel computing, granularity is a qualitative measure of the ratio of computation to communication.
- `Coarse grained`: relatively large amounts of computational work are done between communication events
- `Fine grained`: relatively small amounts of computational work are done between communication events
- `Observed speedup`: Observed speedup of a code which has been parallelized, defined as:
  S = T(serial) / T(parallel) where T(serial) Wall-clock time of serial execution and T(parallel) Wall-clock time of parallel execution
- `Parallel overhead`: The amount of time required to coordinate parallel tasks, as opposed to doing useful work. Parallel overhead includes:
Task start-up time
Synchronizations
Data communication
Software overhead imposed by parallel languages, libraries, operating systems, etc. Task termination time

- `Massively parallel`: Refers to the hardware that comprises a given parallel system - having many processing elements. The meaning of "many" keeps increasing, but currently, the largest parallel computers are comprised of processing elements numbering in the hundreds of thousands to millions (e.g. GPUs).
- `Embarrassingly parallel`: Solving many similar, but independent tasks simultaneously; little to no need for coordination between the tasks.
- `Scalability`: Refers to a parallel system's (hardware and/or software) ability to demonstrate a proportionate increase in parallel speedup with the addition of more resources. Factors that contribute to scalability include: Hardware - particularly memory-cpu bandwidths and network communication properties, Application algorithm, Parallel overhead related, Characteristics of your specific application, and coding

## Computer characterization
### Serial Computing
A serial program consists of a series of instructions that are processed in sequential order (one after the other). They are executed on a single processor (entity that can perform work) Only one instruction can be executed at any time

### Parallel Computing
A problem is divided into discrete chunks that can be solved concurrently
Each chunk is processed sequentially (but many chunks run simultaneously on different processors)
Requires a control/communication/coordination mechanism


## Why do we need Parallel Computing?

**Moore's Law**: states that integrated circuit (IC) (i.e. the number of transistors on a microchip) resources double every 18-24 months.

### Challenges to Moore's Law
1. **The Power Wall:** A major obstacle is the "power wall," which arises due to the increase in power consumption and heat generation as more transistors are packed into chips. As devices shrink, they become more power-efficient per transistor, but the overall chip's power density increases due to more transistors switching at higher frequencies. 

2. **Voltage Reduction and Leakage**: reducing voltage to save power makes transistors leakier, leading to significant energy dissipation even when transistors are not switching.

3. **Transistors and Heat:** With the reduction of transistor size, managing the heat produced by billions of transistors becomes increasingly difficult. High power density means that cooling solutions, especially for large systems like supercomputers, become complex and costly.

### Addressing the Challenges

1. **Reducing Voltage:** Voltage has been reduced by about 15% per generation, from 5V to around 1V, to manage power consumption. However, this leads to increased leakage current.

2. **Sophisticated Cooling Solutions:** To manage the heat, advanced cooling technologies are required, especially as clock rates increase.

3. **Increasing Processor Cores:** Instead of solely focusing on increasing clock speeds, adding more processor cores allows for parallel processing, improving performance without significantly raising power consumption or heat generation.

### Reinterpreting Moore's Law

In response to these challenges, Moore's Law has been reinterpreted from a focus on transistor density to an emphasis on multicore processors. 

- **Multicore Era:** The number of cores per microprocessor doubles every two years, but clock speeds have plateaued and, in some cases, may decrease.
- **Parallel Computing:** The increase in cores necessitates advancements in managing millions of concurrent threads, as seen in GPUs (Graphics Processing Units), and requires efficient use of intra-chip and inter-chip parallelism through shared and distributed memory systems.


## FLYNN'S TAXONOMY: Classification of parallel computers

**SISD (Single Instruction, Single Data)**: processing one instruction on one data stream at a time. Example: IBM 360 the oldest computer architecture type.

**SIMD (Single Instruction, Multiple Data)**: parallel computers that execute the same instruction on multiple data streams simultaneously.
Application: Well-suited for tasks with high regularity, such as graphics and image processing.
Examples: Thinking Machines CM-2, MasPar MP-1 & MP-2 (Processor Arrays); Cray X-MP, Y-MP & C90 (Vector Pipelines). Modern GPUs also incorporate SIMD instructions.

**MISD (Multiple Instruction, Single Data)**
A theoretical model where multiple instructions operate on a single data stream independently.
Application: Possible uses include redundancy for critical missions, applying multiple frequency filters to a single signal, or multiple cryptography algorithms deciphering a single coded message. However, few, if any, practical examples of MISD computers exist.

**MIMD (Multiple Instruction, Multiple Data)**
Parallel computers where each processor may execute different instructions on different data streams.
Execution: Can be synchronous or asynchronous, deterministic or non-deterministic.
Application: The most common parallel computing architecture, encompassing most modern supercomputers and multi-core PCs.
Examples: Supercomputers like Piz Daint (Switzerland) and Summit (ORNL, USA), and multi-processor systems and multi-core PCs. Many MIMD architectures include SIMD components as well.
