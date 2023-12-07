#include "function.h"

/*void exec_complex_command_redirection(char command[]) {
    char *args[MAXSIZE];  // Array to store arguments

    char *token = strtok(command, " "); // Initialize token inside the loop of the child process;
    int argument_index = 0; // Reset argument_index

    while (token != NULL) {
        args[argument_index++] = token;
        token = strtok(NULL, " ");
    }
    args[argument_index] = NULL;  // The last element must be NULL for execvp

    // Search for input and output redirection symbols
    for (int i = 0; args[i] != NULL; ++i) {
        if (strcmp(args[i], "<") == 0) {
            // Input redirection
            int fdIn = open(args[i + 1], O_RDONLY);
            if (fdIn == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            if (dup2(fdIn, STDIN_FILENO) == -1) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fdIn);

            // Remove '<' and the filename from args
            for (int j = i; args[j] != NULL; ++j) {
                args[j] = args[j + 2];
            }
        } else if (strcmp(args[i], ">") == 0) {
            // Output redirection
            int fdOut = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fdOut == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            if (dup2(fdOut, STDOUT_FILENO) == -1) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fdOut);

            // Remove '>' and the filename from args
            for (int j = i; args[j] != NULL; ++j) {
                args[j] = args[j + 2];
            }
        }
    }

    // Execute the command with its arguments
    execvp(args[0], args);
    // If execvp fails, print an error message
    perror("execvp");
    exit(EXIT_FAILURE);
}*/

int main(void) {
    char command[MAXSIZE];
    int numberOfChar;
    int status;
    char buff_exit[MAXSIZE];
    char buff_time[MAXSIZE];
    struct timespec timeStart, timeEnd;

    welcome();

    while (1) {
        // Read user input
        numberOfChar = read(STDIN_FILENO, command, MAXSIZE);
        if (numberOfChar == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        command[numberOfChar - 1] = '\0';

        // Exit if the user types 'exit'
        if (strcmp(command, "exit") == 0) {
            if (write(STDOUT_FILENO, exit_msg, strlen(exit_msg)) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            break;
        }

        // exit when user type ctrl+D
        if (numberOfChar == 0 || (numberOfChar == 1 && command[0] == '\n')) {
            if (write(STDOUT_FILENO, "Goodbye!\n", strlen("Goodbye!\n")) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            break;
        }

        clock_gettime(CLOCK_MONOTONIC, &timeStart); // Capture start time
        pid_t ret = fork();

        if (ret == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (ret == 0) {// Child process code
            exec_complex_command_redirection(command);

            // If execvp fails, print an error message
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(EXIT_FAILURE);

        } else {
            // Parent process code
            if (wait(&status) == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
            }
            clock_gettime(CLOCK_MONOTONIC, &timeEnd);

            display_execTime_exitCode_prompt(status, buff_exit, buff_time, timeStart, timeEnd);
        }
    }
    return 0;
}
