#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NANO 100000000 //use to conver time to nanosecond
#define EXP 10000 //experiment time, easier for data analyze
#define CLOCK CLOCK_PROCESS_CPUTIME_ID //clockid make code shorter

void bare_fun()//the empty function
{
}

unsigned long long exc_time(struct timespec *start, struct timespec *stop)
{
    return ((stop->tv_sec - start->tv_sec)*NANO + stop->tv_nsec - start->tv_nsec);
}//calculate the execution time for this time

void min_fun_cost()//costs of function call
{
    struct timespec fstart, fstop;
    unsigned long long fsum, fmin;
    int i;
    for(i = 0; i < EXP; i++){
        clock_gettime(CLOCK, &fstart);//begining
        bare_fun();//call the bare function
        clock_gettime(CLOCK, &fstop);//end
        fsum += exc_time(&fstart, &fstop);//sum up the result
    }
    fmin = fsum/EXP;//find the average aka the minimal
    printf("The minimal function call in nanosecond is: %llu\n", fmin);
}

void min_sys_cost()//costs of system call
{
    struct timespec sstart, sstop;
    unsigned long long ssum, smin;
    int i;
    for(i = 0; i < EXP; i++){
        clock_gettime(CLOCK, &sstart);
        getpid();//call the system command
        clock_gettime(CLOCK, &sstop);
        ssum += exc_time(&sstart, &sstop);
    }
    smin = ssum/EXP;
    printf("The minimal system call in nanosecond is: %llu\n", smin);
}

void min_rw_cost()//context transfer costs
{
    struct timespec rwstart, rwstop;
    unsigned long long rwsum, rwmin;
    int i;
    int val = 1;//the byte use for transfer
    int fd[2];//set up the pipe
    pipe(fd);

    for(i = 0; i < EXP; i++){
        if(fork() != 0){//parent procdss
            close(fd[0]);//write only, prevent read
            clock_gettime(CLOCK, &rwstart);
            write(fd[1], &val, sizeof(val));
            clock_gettime(CLOCK, &rwstop);
            close(fd[1]);//close write
        }
        else{//child
            close(fd[1]);//read only, prevent write
            read(fd[0], &val, sizeof(val));
            close(fd[0]);//close read
            exit(0);
        }
        rwsum += exc_time(&rwstart, &rwstop);
    }
    rwmin = rwsum/EXP;
    printf("The minimal read and write in nanosecond is: %llu\n", rwmin);
}

int main()//main function, call each of above function
{
    printf("The experiment will repeated %d times\n\n", EXP);
    min_fun_cost();
    min_sys_cost();
    min_rw_cost();
    return 0;
}