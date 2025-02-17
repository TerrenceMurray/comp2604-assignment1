#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void create_processes(int depth, int fds[]);
void read_pids(int fds[], int *even, int *odd, int *multiple10);

int main () {
    int fds[2];

    // create pipe
    if (pipe(fds) == -1) {
        perror("pipe() failed. \n");
        exit(1);
    }

    // create processes
    create_processes(9, fds);

    close(fds[1]); // close the write end of pipe in the parent

    
    // parent process reads PIDs from the pipe
    
    int even = 0, odd = 0, multiple10 = 0;

    int buffer, process_line = 1;

    read_pids(fds, &even, &odd, &multiple10);

    close(fds[0]); // close read end of pipe when done

    // classify parent process
    pid_t pid = getpid();

    printf("PID: %d\n", pid);
    if (pid % 2 == 0)
            even++;
        else 
            odd++;

        if (pid % 10 == 0)
            multiple10++;

    // Output the results
    if (odd > 0 && even > 0 && multiple10 > 0)
        printf("\nEven PIDs: %d\nOdd PIDs: %d\nMultiples of 10 PIDs: %d\n", even, odd, multiple10);

    return 0;
}

void create_processes(int depth, int fds[]) {
    // base case
    if (depth == 0)
        return;

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork() failed. \n");
        exit(1);
    }

    if (pid == 0) {
        // child process
        close(fds[0]); // close the read end of pipe

        int id = getpid();
        write(fds[1], &id, sizeof(id));
    }

    // parent continues to fork
    create_processes(depth - 1, fds);

    wait(NULL);
}

void read_pids(int fds[], int *even, int *odd, int *multiple10) {
    int buffer;
    if (read(fds[0], &buffer, sizeof(buffer)) > 0) {
        printf("PID: %d\n", buffer);
        if (buffer % 2 == 0)
            (*even)++;
        else 
            (*odd)++;

        if (buffer % 10 == 0)
            (*multiple10)++;

        read_pids(fds, even, odd, multiple10);
    }
}
