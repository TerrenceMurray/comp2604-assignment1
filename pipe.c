#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

// function prototypes
void write_int_to_pipe(int fds[], int *value);
int read_int_from_pipe(int fds[]);
void child_process(int fds_1[], int fds_2[]);

// main
int main (int argc, char* argv[]) {

    // check for an argument
    if (argc < 2) {
        perror("provide an interger argument");
        exit(1);
    }

    // create pipes
    int fds_1[2], // parent -> child 
        fds_2[2]; // child -> parent
    
    if (pipe(fds_1) == -1 || pipe(fds_2) == -1) {
        perror("pipe() failed\n");
        exit(1);
    }

    // create a child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork() failed\n");
        exit(1);
    }

    if (pid == 0) {
        // child process

        child_process(fds_1, fds_2);

        exit(0);
    } else if (pid > 0) {
        // parent process

        // covert the value to an integer
        int input, output;
        // (Referenced) https://www.geeksforgeeks.org/c-program-for-char-to-int-conversion/
        sscanf(argv[1], "%d", &input);

        write_int_to_pipe(fds_1, &input);

        output = read_int_from_pipe(fds_2);
        if (output > -1)
            printf("%d\n", output);
    }

    return 0;
}

// function defintions
void child_process(int fds_1[], int fds_2[]) {
    // read the value from the pipe
    int value = read_int_from_pipe(fds_1);

    // process value

    if (value >= 1 && value <= 10) {
        // calculate and return its square

        value = pow(value, 2);
    } else if (value >= 11 && value <= 99) {
        // determine whether it is divisible by 2 and return the result

        if (value % 2 == 0) {
            value = value / 2;

        }
        else {
            printf("%d is not divisible by 2\n", value);

            value = -1;

        }
    } else if (value > 100) {
        // reverse its digits and return the reversed value
        char value_str[100];
        
        sprintf(value_str, "%d", value);
        int length = strlen(value_str);

        // reverse string
        char reversed_str[strlen(value_str) + 1];

        int i = 0;
        while (i < length) {
            reversed_str[length - i - 1] = value_str[i];
            i++;
        }
        reversed_str[length] = '\0';

        sscanf(reversed_str, "%d", &value);
    } else {
        value = -1;
        printf("argument out of range\n");
    }

    write_int_to_pipe(fds_2, &value);
}

int read_int_from_pipe(int fds[]) {
    close(fds[1]); // close the write end

    int value;
    read(fds[0], &value, sizeof(value));
    close(fds[0]); // finished reading from pipe

    return value;
}

void write_int_to_pipe(int fds[], int *value) {
    close(fds[0]); // close the read end

    write(fds[1], value, sizeof(int));
    close(fds[1]); // finished writing to pipe
}