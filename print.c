#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main () {

    printf("Welcome Message \n");
    printf("Welcome Message \n");
    printf("Welcome Message \n");

    pid_t pid1 = fork();
    pid_t pid2 = fork();
    pid_t pid3 = fork();

    printf("Welcome Message \n");
    wait(NULL);

    return 0;
}