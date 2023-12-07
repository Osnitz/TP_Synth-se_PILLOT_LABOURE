#include "function.h"

/*void display_execTime_exitCode_prompt(int status, char *buff_exit, char *buff_time, struct timespec start, struct timespec end) {
    // Computing the program's duration
    long time_seconds = end.tv_sec - start.tv_sec;
    long time_nanoseconds = end.tv_nsec - start.tv_nsec;

    // Choosing unit of time to display and conversion of nanoseconds to milliseconds
    if (time_seconds < 1) {
        snprintf(buff_time, sizeof(buff_time), "| %ld ms]", time_nanoseconds / 1000000);
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

        // exit when the user types ctrl+D
        if (numberOfChar == 0 || (numberOfChar == 1 && command[0] == '\n')) {
            if (write(STDOUT_FILENO, "Goodbye!\n", strlen("Goodbye!\n")) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            break;
        }

        // Start time of the program
        clock_gettime(CLOCK_MONOTONIC, &timeStart);

        pid_t ret = fork();

        if (ret == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (ret == 0) {
            // Child process code
            execlp(command, command, NULL);

            // If execlp fails, print an error message
            if (write(STDOUT_FILENO, error_msg, strlen(error_msg)) == -1) {
                perror("write");
            }
            exit(EXIT_FAILURE);
        } else {
            // Parent process code
            if (wait(&status) == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
            }

            // End time of the program
            clock_gettime(CLOCK_MONOTONIC, &timeEnd);
            display_execTime_exitCode_prompt(status, buff_exit, buff_time, timeStart, timeEnd);
        }
    }
    return 0;
}
