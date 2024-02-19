
  LAST TIME
Hardware organization of typical computer
TODAY
Main topic: Cache memories
Details:
● Why do we need caches?
● Principle of locality
● Strided memory accesses
● High-level approach of how caches work ● Cache lines/blocks
● The different types of cache misses
● Example to analyze cache misses in a given code
SCHEDULE CHECK:
  Classic von Neumann architecture and memory issues
Virtual memory and virtual address space of Linux process
  •
•
See https://harvard-iacs.github.io/2024-CS205/pages/project.html#M1 for the details. Due February 6th 11:59pm.
Quiz 1 this Friday covers lectures 1-3.
Project milestone 1  form project teams (please aim at teams of 4. Teams of 2 or less are not acceptable).
 
 LIBRARY PARADIGM
Assume you are writing an important review of computer hardware You go to the local library to work undisturbed
  How do you organize yourself in the library?
    Do you get one book or a few at a time from the main library?
Put them on your desk where you sit down and work until you find you are missing a book
Of course, you get as many you can!

 CACHE MEMORIES
● The same happens in a CPU with caches
● Recall the classic von Neumann architecture of last time:
CPU
  Register file
Bus interface
  ALU
     Classic von Neumann architecture
System bus
I/O bridge
Memory bus
Main memory
  von Neumann architecture with on-chip caches
● Caches are small on-chip memories, much like the space you have
on your desk where you put your books in the library example before
In most architectures there are three levels of caches:
○ L1: Smallest and closest cache to the ALU. Most often 32kB in size and exclusive to core
○ L2: Intermediate cache after L1. Often 256-512kB in size but can be larger. Often shared
among a few cores.
○ L3: Last level cache (LLC) closest to DRAM. Size varies on type of CPU (desktop,
workstation/compute node) and may be in the range of 4-30MB. Typically shared among cores.
   
 CACHE MEMORIES
Laptop: Compute node on old HPC cluster:
AMD Ryzen 7 PRO 4750U (released 2020)
AMD Opteron 6376 (released 2012) Do you notice anything different between these two CPUs?
                                                                                       
 CACHE MEMORIES
● There are two level 1 caches (on all modified von Neumann type CPUs you can find):
○ L1d: Level 1 data cache (exclusive for data)
○ L1i: Level 1 instruction cache (exclusive for instructions). There is a separate
L1 instruction cache because of different access patterns and pipelines. We
will see more about this when we talk about instruction-level parallelism.
● The level 2 and 3 caches are unified. They can cache either data or
instructions. Recall the latencies associated with these cache levels (from last time):
  Type
CPU registers L1 cache
L2 cache
L3 cache
Where cached
On-chip CPU registers On-chip L1 cache On-chip L2 cache On-chip L3 cache
Latency Managed by (cycles)
0 Compiler
4 Hardware 10 Hardware 50 Hardware
     
 CACHE MEMORIES
So why are there multiple level caches and not just one big cache?
            Smaller, faster, and costlier (per byte) storage devices
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
  Larger, slower, and cheaper (per byte) storage devices
L6:
L4:
      L5:
Local secondary storage (local disks)
                          Remote secondary storage (distributed file systems, Web servers)
      
 CACHE MEMORIES: TAKE-AWAY
One of the most important take-aways from CS205 is that application programmers who are aware of cache memories can exploit the data locality in caches to improve the performance of their programs by an order of magnitude.
The proof was in the last reading assignment:
                                                                                                            Leiserson et al., Science 2020

 THE PRINCIPLE OF LOCALITY
• Well written programs exhibit good locality
• The principle of locality has a strong impact on the design and
performance of a program
● A program with good locality references data items that are spatially near other recently referenced items or were recently referenced themselves (temporal locality)
• These two cases are known as spatial and temporal locality
• From the memory pyramid we learned that every memory level in a
computer system acts as a cache which is designed to exploit locality
● Programs with good locality run faster than programs with poor locality!
  
 TEMPORAL LOCALITY
• A variable that is referenced frequently in a short time interval is said to have good temporal locality
• Given the array
int v[N]; // N elements in the array with the following memory layout:
where the numbers in the blocks indicate the array indices. Note that in this example each of the numbered blocks is 4 byte in size.
    0
  1
2
  3
 4
5
  6
7
  8
 9
10
  11
12
  13
14
 15
  16
17
  18
 19
20
  21
22
  23
       • Consider now the following code:
• Does sum exploit good temporal locality?
• Does v[i] exploit good temporal locality?
 int sum = 0;
for (int i = 0; i < N; ++i) {
sum += v[i]; }

 Given the array
TEMPORAL LOCALITY
 int v[N]; // N elements in the array with the following memory layout:
where the numbers in the blocks indicate the array indices. Note that in this example each of the numbered blocks is 4 byte in size.
Consider this nested loop:
   0
  1
 2
3
  4
5
  6
 7
8
  9
10
  11
12
 13
  14
15
  16
 17
18
  19
20
  21
22
  23
         for (int loop = 0; loop < 10; ++loop) { for (int i = 0; i < N; ++i) {
}
  ... = ... v[i] ...;
}
What is the temporal locality of v[i] What is the temporal locality of v[i] this example? in this example?
In the library paradigm you took the book to your work desk because you will use it frequently
for (int i = 0; i < N; ++i) {
for (int loop = 0; loop < 10; ++loop) {
}
... = ... v[i] ...;
}
     0
  1
2
  3
 
 SPATIAL LOCALITY
• A program is said to exhibit spatial locality if it references memory "close" to memory it already referenced
• In the classic von Neumann architecture spatial locality is irrelevant because any memory location retrieved as quickly as another one. (DRAM → RAM stands for random access memory →latency to access any memory cell the same.)
• Spatial locality is relevant in modern CPUs with caches. Recall the library paradigm: you got multiple books and put them on your desk. Libraries put books on similar topics nearby to increase spatial locality. (You grab a bunch of books all next to each other for efficiency → concept of a cache line see later slides.)
• Recall this loop from before:
The variable v[i] exhibits good spatial locality because every element i access is immediately followed by i-1 (related books are next to each other in the shelves)
• A sequential access pattern like in the loop above is called stride-1 reference pattern (sometimes sequential reference pattern or unit stride pattern)
 int sum = 0;
for (int i = 0; i < N; ++i) {
sum += v[i]; }
    0
 1
  2
 3
   
 STRIDED MEMORY ACCESS
• Algorithms may not always allow for unit stride access patterns
• A stride-k reference pattern visits every k-th element of a contiguous vector
• The larger k, the smaller the spatial locality you exploit (cache size is limited!)
• Stride-4 reference pattern:
• Stride-12 reference pattern:
• Strided reference patterns are a concern in higher-dimensional stencil schemes. Consider solving the Poisson equation 2u = f . In 2D, the finite difference
    0
1
  2
 3
4
  5
 6
7
  8
 9
10
  11
12
 13
  14
15
  16
17
 18
  19
 20
21
  22
 23
          0
1
  2
 3
4
  5
 6
7
  8
 9
10
  11
12
 13
  14
15
  16
17
 18
  19
 20
21
  22
 23
       stencil on a uniform N × N grid with size h is:
2u ≈ 1 (ui+1,j + ui−1,j + ui,j+1 + ui,j−1 −4ui,j)
i,j h2
What is k in this stride-k access pattern?
 
 HIGHER DIMENSIONAL ARRAYS
All memory in a computer is a contiguous vector of bytes How are higher-dimensional arrays represented in memory?
Two main distinctions: row-major and column-major order Row-major: used in C or C++
0 1 2 3 Layout in memory
     4567
8 9 10 11 12 13 14 15
Column-major: used in Fortran 0123
Layout in memory
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
           4567
8 9 10 11 12 13 14 15
0 4 8 12 1 5 9 13 2 6 10 14 3 7 11 15
 The ordering follows the same pattern for more than two dimensions

 •
INDEXING INTO A MULTI-DIMENSIONAL ARRAY
You are given the following code: In memory the array looks like:
You can index this 2D array like:
double a_ij = A[i][j];
and the compiler knows how to translate the index-pair i,j into a flat (linear) index
In row-major order, index j is the fast-moving index
In column-major order the fast-moving index is i
When you allocate multi-dimensional arrays on the heap use
 #define N 4
double A[N][N]; // 2D-array
  0
  1
 2
 3
4
  5
 6
 7
8
  9
 10
 11
12
  13
 14
 15
     • •
• • •
   How do you compute this flat index?
 double a_ij = A[...];
 double *A = new double[N * N]; // one contiguous chunk of memory // double **A = new double[N][N]; // THIS IS WRONG!!

 EXAMPLES OF LOCALITY
Which of the two codes below exhibits better spatial locality for a row-major order (referencing elements that are nearby in memory)?
Code A: Code B:
  float A[N][N]; float sum = 0.0;
for (int i = 0; i < N; ++i) {
for (int j = 0; j < N; ++j) {
       sum += A[i][j]
    }
}
Note that the above code A can be written with a single loop:
float A[N][N]; float sum = 0.0;
for (int j = 0; j < N; ++j) {
for (int i = 0; i < N; ++i) {
        sum += A[i][j]
    }
}
 float *p = &A[0][0]; float sum = 0.0;
for (int i = 0; i < N * N; ++i) {
sum += p[i]; // element access is identical
}

 EXAMPLES OF LOCALITY
The matrix-matrix multiplication C = AB can be computed in different ways. Assume all matrices are stored in row-major order and there is no cache (for now). How does the temporal and spatial locality differ from the following two implementations:
Implementation 1 Implementation 2
  for (int i = 0; i < N; ++i) {
for (int j = 0; j < N; ++j) {
for (int k = 0; k < N; ++k) { C[i][j] += A[i][k] * B[k][j]
} }
}
jj i=ii=i
k
      C=AB
Computes the dot-product in the inner-most loop
C=AB
for (int i = 0; i < N; ++i) {
for (int k = 0; k < N; ++k) {
for (int j = 0; j < N; ++j) { C[i][j] += A[i][k] * B[k][j]
} }
}
     Updates rows of C by scaling rows of B with elements of A
k

 EXAMPLES OF LOCALITY
Implementation 1 Implementation 2
jj i=ii=i
k
           C=AB
C[i][j]
k C=AB
In the first implementation C[i][j] is invariant and has good temporal locality (will be stored in register). In the second implementation we have to load C[i][j] in each inner iteration (this implies more store operations).
A[i][k]
Both implementations address elements in A by rows (good spatial locality). For the second implementation A[i][k] is invariant w/r/t to inner-most loop (good temporal locality). The first implementation must load the elements in A the same number of times as we must store C[i][j] in implementation 2, so both implementations are even up to here.
B[k][j]
The magic difference is the access pattern for B. The second implementation exhibits better spatial locality than the first implementation. Especially when caches are involved, the access pattern of the first implementation is bad because we have to load cache lines.
 
 CACHES
• For each level k in the memory hierarchy, a faster and smaller storage device serves as a cache for the next storage level k + 1
• Information in level k + 1 is partitioned into blocks of data and is transferred in the same block-granularity to the next level
• Blocks can be fixed size (usually the case) or variable size
• L1 and L2 caches interact as determined by the cache inclusion policy
Data between levels is transferred in blocks
                                               
 Cache block/line
CACHES
Terminology:
Fixed size packet that moves back and forth between cache levels or main memory. Often called cache line. A cache line is a container that stores a block as well as other information such as the valid and tag bits (not important for us).
Cache hit
Data request by the processor that is found in level k. In that case the data is
read directly from the level k cache which is faster than level k + 1. Cache miss
If the data is not found, then it is a cache miss. In this case a new cache line must be fetched from the level k + 1 cache. This line must evict an existing line in level k (the victim) according to a replacement policy. Possible policies are random replacement, least recently used (LRU) or least frequently used (LFU) policies for example.
Hit/miss rate
Fraction of memory accesses that are cache hits (or misses) in the level k cache. Miss penalty
Time to replace a block in level k with the corresponding block from level k + 1.
 
 CACHE LINES
Library paradigm: you take whole books to your desk, not just single pages. A cache line is usually tens of bytes. On most Intel and AMD systems it is
64 byte → n-byte cache lines are aligned at n-byte boundaries!
 When you access a data element in your data structure, multiple coalesced elements are loaded at once, not just one.
 64 byte are:
16 elements of 4-byte types (int or float for example)
8 elements of 8-byte types (long int or double for example)
Recall: implementation 1 of previous matrix-matrix multiplication example:
jj
ii =
C=AB
Each access B[k][j] loads a cache line From the, say, 8 loaded elements, we
only use 1!
This is inefficient and wastes a lot of bandwidth!
   Cache lines
    
 THE 3 C'S
There are three different type of cache misses (the 3 C's): Compulsory or cold misses
An empty cache is sometimes referred as a cold cache. Misses of this kind are called compulsory or cold misses. You cannot avoid these misses. Compulsory misses are further transient, i.e. they have some initial effects which is why you should warm up the cache when you are collecting benchmark data.
Capacity misses
Because the cache size is finite, it can only hold a finite number of cache lines. When your working set (e.g. the array you currently work with) is larger than the cache, you will experience capacity misses because older cache lines need to be evicted to make room for the new ones. This causes reloading data in caches when you return to a previous index in your working set (e.g. in a loop).
Conflict misses
These are the hardest to understand. For efficiency reasons, caches have placement policies for cache lines. It can happen that two cache lines compete for a spot in the cache even if the cache is not full.

 THE 3 C'S
Assume a cache line can hold 4 data elements and the L1 cache is 32kB in size:
 1 double A[8192]; // 65kB does not fit in L1
2 double sum = 0.0;
3
4 for(inti=0;i<8192;++i){
5 6}
sum += A[i];
Array A looks like this in memory:
 0123
 4567
 8 9 10 11
 12 13 14 15
 16 17 18 19
 20 21 22 23
...

 THE 3 C'S
Assume a cache line can hold 4 data elements and the L1 cache is 32kB in size:
 1 double A[8192]; // 65kB does not fit in L1
2 double sum = 0.0;
3
4 for(inti=0;i<8192;++i){
5 6}
sum += A[i];
Array A looks like this in memory:
20 21 22 23... Cold miss (load cache line from DRAM)
 0123
 4567
 8 9 10 11
 12 13 14 15
 16 17 18 19
  0123
i = 0:

 THE 3 C'S
Assume a cache line can hold 4 data elements and the L1 cache is 32kB in size:
 1 double A[8192]; // 65kB does not fit in L1
2 double sum = 0.0;
3
4 for(inti=0;i<8192;++i){
5 6}
sum += A[i];
Array A looks like this in memory:
20 21 22 23...
Cold miss (load cache line from DRAM)
Cache hit Cache hit Cache hit
 0123
 4567
 8 9 10 11
 12 13 14 15
 16 17 18 19
  0123
 0123
 0123
 0123
i = 0: i = 1: i = 2: i = 3:

 THE 3 C'S
Assume a cache line can hold 4 data elements and the L1 cache is 32kB in size:
 1 double A[8192]; // 65kB does not fit in L1
2 double sum = 0.0;
3
4 for(inti=0;i<8192;++i){
5 6}
sum += A[i];
Array A looks like this in memory:
20 21 22 23 ...
Cold miss (load cache line from DRAM)
Cache hit
Cache hit
Cache hit
Cold miss (load cache line from DRAM) Cache hit
Continues until cache is full at 32kB. From that point on we have capacity misses.
 0123
 4567
 8 9 10 11
 12 13 14 15
 16 17 18 19
  0123
 0123
 0123
 0123
i = 0: i = 1: i = 2: i = 3: i = 4: i = 5:
 4567
 4567
...

 THE 3 C'S
Assume we have a more involved access pattern like this: (The finite difference stencil for the Poisson equation we saw before has a similar data access pattern.)
Array A still looks the same:
Here is where spatial locality kicks in!
...
 1 for(inti=1;i<8191;++i){
2 sum += A[i-1] + A[i] + A[i+1]; 3}
 0123
 4567
 8 9 10 11
 12 13 14 15
 16 17 18 19
 20 21 22 23
 0123
 0123
 0123
 0123
Good spatial locality increases the hit rate in your cache and therefore the bandwidth with which you service the CPU!
A well written code (optimized) maximizes the green and minimizes the red. This will improve the performance of your application already dramatically.
 4567
 4567
 4567
 4567
...

 PLACEMENT POLICIES FOR CACHE LINES
• Cache lines are replaced based on a policy
• The most flexible policy is to allow a block from level k + 1 to be placed
anywhere in level k
• This is too expensive where speed matters (close to the CPU)
• Hardware typically implements a restriction where a block from level k + 1
can only be placed in a small subset of the blocks in level k
• Recall the figure from earlier:
                                              
 PLACEMENT POLICIES FOR CACHE LINES
Data layout in memory (bold white numbers are cache line indices):
Address:0 4 8 12 16 20 24 28 Assume the cache can hold 4 cache blocks (16 elements in total)
 0
 1
 2
 3
 4
 5
 6
 7
Fully associative cache:
• Cache lines can be placed anywhere
• Expensive to build Cache block order:
(there are many other possibilities) Block 0:
Block 1: Block 2: Block 3:
Direct mapped cache:
n-way set associative cache:
• •
Cache blocks map to the same location (simplest) Prone to conflict misses
Cache block order:
Block 0: Block 1: Block 2: Block 3:
• •
Cache blocks map to a set of n possible locations
Reduces conflict misses
Cache block order: (2-way set associative example)
    4
7
2
3
 0/4
 1/5
 2/6
 3/7
 0/2/4/6
 0/2/4/6
 1/3/5/7
 1/3/5/7
Block 0: Block 1: Block 2: Block 3:

 • •
•
CONFLICT MISSES
Assume we have two arrays: array0 starts at address 0 and array1 starts at address 16 A conflict miss happens when you reference an element in array0 and another element in array1 for which the cache blocks map to the same cache location.
Conflict misses lead to cache thrashing which causes serious performance degradation
Address:0 4 8 12 16 20 24 28
  0
 1
 2
 3
 4
 5
 6
 7
    array0
Direct mapped cache example
Conflict miss
array1
   0/4
 1/5
 2/6
 3/7
 0/2/4/6
 0/2/4/6
 1/3/5/7
 1/3/5/7
•
n-way set associative caches reduce this problem because the hardware can choose from n possible locations (within a set) where to put the block in the cache
Block 0: Block 1: Block 2: Block 3:
Block 0: Block 1: Block 2: Block 3:
2-way set associa ve cache example

 CONFLICT MISSES
Adverse performance impact due to cache thrashing on AMD Ryzen 7:
                  cycle
Conflict misses observed with the AMD Ryzen 7 PRO 4750U CPU on a laptop.
8-way set associative L1 cache with 32kB capacity.
Conflict misses are prone to array (byte) sizes that are a power of two (why?)
“Cache resonance” caused by perfect alignment of arrays mapping onto exactly the same locations in the associative cache

 EXAMPLE: CACHE MISSES IN DOUBLY NESTED LOOP
Assume a fully-associative cache can hold 4 cache blocks. Each cache block can hold 4 single precision floating point numbers such that the cache can hold 16 floats in total. Determine the number of any cache misses and the hit rate in the following C++ codes:
Code A: Code B:
  1
float A[8][8]; // 64 float numbers
float sum = 0.0;
2
3
4
5
6 7} 8}
for(intj=0;j<8;++j){
for (int i = 0; i < 8; ++i) {
sum += A[i][j];
1
float A[8][8]; // 64 float numbers
float sum = 0.0;
2
3
4
5
6 7} 8}
for(inti=0;i<8;++i){
for (int j = 0; j < 8; ++j) {
sum += A[i][j];

 A[8][8]
CASE A
    0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
i
Cache
      j
The cache starts initially empty
 
 A[8][8]
CASE A
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
   Cache
 0
     j=0,i=0 Miss
This is an unavoidable cold miss

 A[8][8]
CASE A
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
Cache
    0
  2
   j=0, i=0 Miss j=0, i=1 Miss

 A[8][8]
CASE A
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
Cache
   0
 4
 2
    j=0, i=0 Miss j=0, i=1 Miss j=0, i=2 Miss
Given the policy, blocks are assigned randomly

    0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
 10
 164
 8
 12
     j=0, i=0 Miss j=0, i=1 Miss j=0,i =2 Miss
j=0, i=7 Miss
Capacity miss! Once the cache is full, blocks are evicted
A[8][8]
CASE A
Cache
...

    0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
 12
 10
 8
 164
     j=0, i=0 Miss j=0, i=1 Miss j=0, i=2 Miss
j=0, i=7 Miss
j=3, i=0 Miss j=3, i=1 Miss j=3, i=2 Miss
j=3, i=7 Miss
At each iteration, every block has to be loaded again (bad)
A[8][8]
CASE A
Cache
...
...

 A[8][8]
CASE A
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
 9
 13
 15
 11
  j=0, i=0 Miss j=0, i=1 Miss j=0, i=2 Miss
j=0, i=7 Miss
j=3, i=0 Miss j=3, i=1 Miss j=3, i=2 Miss
j=3, i=7 Miss
j=7, i=0 Miss j=7, i=1 Miss j=7, i=2 Miss
j=7, i=7 Miss
At no point in the process was the cache utilized effectively
Cache
...
...
...

 EXAMPLE: CACHE MISSES IN DOUBLY NESTED LOOP
Assume a fully-associative cache can hold 4 cache blocks. Each cache block can hold 4 single precision floating point numbers such that the cache can hold 16 floats in total. Determine the number of any cache misses and the hit rate in the following C++ codes:
Code A:
Code B:
  1
float A[8][8]; // 64 float numbers
float sum = 0.0;
2
3
4
5
6 7} 8}
for(intj=0;j<8;++j){
for (int i = 0; i < 8; ++i) {
sum += A[i][j];
• C/C++ is row-major order
• Access pattern has bad spatial
locality 64 cache misses
• 64−64×100%=0%hitrate 64
1
float A[8][8]; // 64 float numbers
float sum = 0.0;
2
3
4
5
6 7} 8}
for(inti=0;i<8;++i){
for (int j = 0; j < 8; ++j) {
sum += A[i][j];

 A[8][8]
CASE B
    0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
i
Cache
      j
The cache starts initially empty
 
 A[8][8]
CASE B
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
   Cache
 0
     i=0, j=0 Miss
We have a compulsory miss

 A[8][8]
CASE B
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
   Cache
 0
     i=0, j=0 Miss i=0, j=1 Hit
Locality is used to get a cache hit

 A[8][8]
CASE B
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
   Cache
 0
     i=0, j=0 Miss i=0, j=1 Hit i=0, j=2 Hit
Locality is used to get a cache hit

 A[8][8]
CASE B
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
   Cache
 0
  1
   i=0, j=0 Miss i=0, j=1 Hit i=0, j=2 Hit i=0, j=3 Hit i=0, j=4 Miss
Another unavoidable cold miss.
The new block is placed anywhere in cache.

 CASE B
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
    0
  1
   i=0, j=0 Miss i=0, j=1 Hit i=0, j=2 Hit i=0, j=3 Hit i=0,j=4 Miss
Each row has only two cold cache misses.
...
i=0, j=7 Hit
A[8][8]
Cache

 CASE B
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
 6
 4
 7
 5
     i=0, j=0 Miss i=0, j=1 Hit i=0, j=2 Hit i=0, j=3 Hit i=0, j=4 Miss
i=3, j=0 Miss i=3, j=1 Hit i=3, j=2 Hit i=3, j=3 Hit i=3, j=4 Miss
Once loaded, each block is utilized fully.
...
...
i=0, j=7 Hit
i=3, j=7 Hit
A[8][8]
Cache

 A[8][8]
CASE B
   0
 1
 2
 3
 4
 5
 6
 7
 8
 9
 10
 11
 12
 13
 14
 15
 13
 15
 14
 12
  i=0, j=0 Miss i=0, j=1 Hit i=0, j=2 Hit i=0, j=3 Hit i=0, j=4 Miss
i=3, j=0 Miss i=3, j=1 Hit i=3, j=2 Hit i=3, j=3 Hit i=3, j=4 Miss
i=7, j=0 Miss i=7, j=1 Hit i=7, j=2 Hit i=7, j=3 Hit i=7, j=4 Miss
This case utilized the cache optimally.
...
...
...
i=0, j=7 Hit
i=3, j=7 Hit
i=7, j=7 Hit
Cache

 EXAMPLE: CACHE MISSES IN DOUBLY NESTED LOOP
Assume a fully-associative cache can hold 4 cache blocks. Each cache block can hold 4 single precision floating point numbers such that the cache can hold 16 floats in total. Determine the number of any cache misses and the hit rate in the following C++ codes:
Code A:
Code B:
  1
float A[8][8]; // 64 float numbers
float sum = 0.0;
2
3
4
5
6 7} 8}
for(intj=0;j<8;++j){
for (int i = 0; i < 8; ++i) {
sum += A[i][j];
• C/C++ is row-major order
• Access pattern has bad spatial
locality 64 cache misses
• 64−64×100%=0%hitrate 64
•
•
C/C++ is row-major order
Access pattern has good spatial locality
16 cache misses
1
float A[8][8]; // 64 float numbers
float sum = 0.0;
2
3
4
5
6 7} 8}
for(inti=0;i<8;++i){
for (int j = 0; j < 8; ++j) {
sum += A[i][j];
• 64−16×100%=75%hitrate 64

 WRITING CACHE BLOCKS TO THE NEXT LEVEL
• After data has been modified or if a cache block gets evicted, it must be written back to the next cache level and eventually to DRAM
• There are two write policies used in caches: Write-through
• This is the simplest approach. If a cache block is modified it is immediately written to the next level in the memory hierarchy (e.g. from L1 to L2) While this is the simplest approach, the disadvantage is that it causes extensive bus traffic with every write.
Write-back
• The write-back strategy defers the update to the next level in memory hierarchy as long as possible. The cache block is only written when it gets evicted from the cache by the replacement algorithm. This approach significantly reduces the bus traffic (because of locality) but has the disadvantage of additional complexity. A dirty bit must be maintained for each cache line to indicate whether the cache line has been modified.

 IMPACT OF CACHE AND BLOCK SIZES
Impact of cache size
A larger cache will tend to increase the hit rate but it is harder to make large memories run fast. As a result, larger caches tend to increase the hit time. This is the reason why an L1 cache is smaller than an L2 and L2 is smaller than L3.
Impact of block (or line) size
Large blocks are good and bad. Larger blocks help increase the hit rate by exploiting spatial locality. Larger blocks also means that fewer blocks fit into the cache which can hurt the hit rate in programs with high fraction of temporal locality. A trade off on modern systems is usually 64 byte per cache block.

 RECAP
• All computers employ caching throughout a memory hierarchy in order to improve bandwidth performance by exploiting locality
• Application programmers aware of cache memories can exploit them through locality and improve the performance of their programs by an order of magnitude
• Principle of locality:
• Temporal locality: reuse the same data element nearby in time
• Spatial locality: reference data elements that are nearby in memory
• Caches and cache lines/blocks
• The three types of cache misses that you have to be aware of (3 C's)
Further reading:
• Sections 2.1, 2.2, 2.3 in Pacheco, An Introduction to Parallel Programming, Morgan Kaufmann, 2011 Chapter 1 in Eijkhout, Introduction to High Performance Scientific Computing, free PDF
• Chapter 1 in Hager and Wellein, "Introduction to High Performance Computing for Scientists and Engineers", CRC Press, 2011
     