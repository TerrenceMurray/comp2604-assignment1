#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void repeat_fork(int aggregates[], int iteration) {
    if (iteration == 0)
        return;

    pid_t pid = fork(); 

    if (pid != 0)
    {
        printf("This is a child process with id %d\n", pid);

        // even
        if (pid % 2 == 0) 
            aggregates[0] = aggregates[0] + 1;
        // odd
        else
            aggregates[1] = aggregates[1] + 1;
        
        // multiple of 10
        if (pid % 10 == 0)
            aggregates[2] = aggregates[2] + 1;

        wait(NULL);
    } 

    repeat_fork(aggregates, iteration - 1);
}

int main () {
    int aggregates[] = {0, 0, 0}; // even_pids, odd_pids, multiples_of_10

    repeat_fork(aggregates, 9);

    printf(
        "Total processes: %d\nTotal even pids: %d\nTotal odd pids: %d\n",
        aggregates[0] + aggregates[1],
        aggregates[0],
        aggregates[1]
    );

    return 0;
}