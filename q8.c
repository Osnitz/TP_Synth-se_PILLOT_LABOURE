#include "function.h"

/* void exec_complex_command_pipe(char command[]) {
    char *token;
    char *args[MAXSIZE];
    int arg_index = 0;

    token = strtok(command, " ");
    while (token != NULL) {
        args[arg_index++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_index] = NULL;

    // Search for input and output redirection symbols
    for (int i = 0; args[i] != NULL; ++i) {
        if (strcmp(args[i], "<") == 0) {
            // Input redirection
            int fdIn = open(args[i + 1], O_RDONLY);
            if (fdIn == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fdIn, STDIN_FILENO); // dup2 to redirect standard input or standard output
            close(fdIn);

            // Remove '<' and the filename from args
            for (int j = i; args[j] != NULL; ++j) {
                args[j] = args[j + 2];
            }
        } else if (strcmp(args[i], ">") == 0) {
            // Output redirection
            int fdOut = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
           // O_WRONLY: specifies that the file should be opened for writing only.
           // O_CREAT: specifies that the file should be created if it doesn't already exist.
           // O_TRUNC: specifies that the file should be truncated to zero length if it exists. This means that
           // the file will be emptied of its previous contents.

if (fdOut == -1) {
perror("open");
exit(EXIT_FAILURE);
}
dup2(fdOut, STDOUT_FILENO);
close(fdOut);

// Remove '>' and the filename from args
for (int j = i; args[j] != NULL; ++j) {
args[j] = args[j + 2];
}
}
}

// Search for pipe symbol '|'
for (int i = 0; args[i] != NULL; ++i) {
if (strcmp(args[i], "|") == 0) {
// Pipe redirection
int pipe_fd[2];
if (pipe(pipe_fd) == -1) {
perror("pipe");
exit(EXIT_FAILURE);
}

pid_t pipe_child = fork();

if (pipe_child == 0) {
// Child process for the command after pipe
close(pipe_fd[1]); // Close write end of the pipe
dup2(pipe_fd[0], STDIN_FILENO); // Redirect standard input to the read end of the pipe
close(pipe_fd[0]); // Close read end of the pipe

// Execute the command after pipe with its arguments
execvp(args[i + 1], &args[i + 1]);
perror("execvp");
exit(EXIT_FAILURE);
} else {
// Parent process for the command before pipe
close(pipe_fd[0]); // Close read end of the pipe
dup2(pipe_fd[1], STDOUT_FILENO); // Redirect standard output to the write end of the pipe
close(pipe_fd[1]); // Close write end of the pipe

// Remove '|' and the command after pipe from args
args[i] = NULL;
break; // Exit the loop after handling the first pipe
}
}
}

// Execute the command with its arguments
execvp(args[0], args);
perror("execvp");
exit(EXIT_FAILURE);
} */

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
            exec_complex_command_pipe(command);

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
