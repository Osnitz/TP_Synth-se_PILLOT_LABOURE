#include "function.h"

void welcome() {
    write(STDOUT_FILENO, welcome_msg, strlen(welcome_msg));
    write(STDOUT_FILENO, exit_instruction, strlen(exit_instruction));
    write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
}

void display_execTime_exitCode_prompt(int status, char *buff_exit, char *buff_time, struct timespec start, struct timespec end) {
    // Computing the program's duration
    long time_seconds = end.tv_sec - start.tv_sec;
    long time_nanoseconds = end.tv_nsec - start.tv_nsec;

    // Choosing unit of time to display and conversion of nanoseconds to milliseconds
    if (time_seconds < 1) {
        snprintf(buff_time, sizeof(buff_time), "|%ld ms]", time_nanoseconds / 1000000);
    } else {
        snprintf(buff_time, sizeof(buff_time), "| %ld s]", time_seconds);
    }

    // Check if the process terminated normally
    if (WIFEXITED(status)) {
        sprintf(buff_exit, "[Code exit : %d ", WEXITSTATUS(status));
        if (write(STDOUT_FILENO, ensea, strlen(ensea)) == -1 ||
            write(STDOUT_FILENO, buff_exit, strlen(buff_exit)) == -1 ||
            write(STDOUT_FILENO, buff_time, strlen(buff_time)) == -1 ||
            write(STDOUT_FILENO, percent, strlen(percent)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    } else if (WIFSIGNALED(status)) {
        sprintf(buff_exit, "[Sign : %d] ", WEXITSTATUS(status));
        if (write(STDOUT_FILENO, ensea, strlen(ensea)) == -1 ||
            write(STDOUT_FILENO, buff_exit, strlen(buff_exit)) == -1 ||
            write(STDOUT_FILENO, percent, strlen(percent)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
}

void exec_complex_command(char command[]) {
    char *args[MAXSIZE];  // Array to store arguments

    char *token = strtok(command, " "); // Initialize token inside the loop of the child process;
    int argument_index = 0; // Reset argument_index

    while (token != NULL) {
        args[argument_index++] = token;
        token = strtok(NULL, " ");
    }
    args[argument_index] = NULL;  // The last element must be NULL for execvp

    // Execute the command with its arguments
    execvp(args[0], args);
    // If execvp fails, print an error message
    perror("execvp");
    exit(EXIT_FAILURE);
}

void exec_complex_command_redirection(char command[]) {
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
}

void exec_complex_command_pipe(char command[]) {
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
            /*O_WRONLY: specifies that the file should be opened for writing only.
            O_CREAT: specifies that the file should be created if it doesn't already exist.
            O_TRUNC: specifies that the file should be truncated to zero length if it exists. This means that
            the file will be emptied of its previous contents.*/

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
}

