#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>

int F_n (int n) {
    int F_0 = 0, F_1 = 1, F_n;

    if (n == 0 || n == 1) return n;

    for (int i = 2; i <= n; i++) {
        F_n = F_0 + F_1;
        F_0 = F_1;
        F_1 = F_n;
    } 

    return F_n;
}

int main () {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork() failed.");
        exit(1);
    }

    if (pid > 0) {
        // parent process
        printf("Parent Process ID: %d\n\n", getpid());

        if (pid % 5 != 0) {
            printf("Child Process ID: %d\t Child process ID is not divisible by 5\n", pid);
            return 0;
        }

        for (int i = 0; i < 12; i++) {
            printf("Child Process ID: %d\t Fibonacci sequence: %d\n", pid, F_n(i));
        }
    }

    return 0;
}