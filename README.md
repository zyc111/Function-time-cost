# Function-time-cost

## Problem Statement goes below:

1.Measure the cost of a minimal function call in C/C++ (cost in terms of time taken). The minimal cost could be emulated by measuring a bare function call that neither takes any parameter nor does anything inside the function. 

2.Measure the cost of a minimal system call in C/C++. Unlike a regular function call, a system call traps into the operating system kernel. The minimal cost can be emulated by measuring the cost of getpid () which doesn't really do anything.

3.Measure the cost of a read/write system call in C/C++. ( Hint : You can fork a child process and transfer one byte between parent and child) For the above measurements, you will need to repeat the experiment many times and then take the average.

## How to tests:

```c
#define EXP 10000 //experiment time, easier for data analyze
```

The above code is the experiment time that can be change to test the result.

The result of test will show a obvious trend which is as the experiment time increase, the execution time for both function call and system call decrease.
