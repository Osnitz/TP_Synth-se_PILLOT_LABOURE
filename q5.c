#include "function.h"

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
        command[numberOfChar - 1] = '\0';

        // Exit if the user types 'exit'
        if (strcmp(command, "exit") == 0) {
            write(STDOUT_FILENO, exit_msg, strlen(exit_msg));
            break;
        }

        // exit when user type ctrl+D
        if (numberOfChar == 0 || (numberOfChar == 1 && command[0] == '\n')) {
            write(STDOUT_FILENO, "Goodbye!\n", strlen("Goodbye!\n"));
            break;
        }

        //Start time of program
        clock_gettime(CLOCK_MONOTONIC, &timeStart); // Capture start time

        pid_t ret = fork();

        if (ret == 0) {
            // Child process code
            execlp(command, command, NULL);

            // If execlp fails, print an error message
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(EXIT_FAILURE);
        } else {
            // Parent process code
            wait(&status);

            //End time of program
            clock_gettime(CLOCK_MONOTONIC, &timeEnd);

            // computation of time and conversion to String, so it can be used in write()
            long time_seconds = timeEnd.tv_sec - timeStart.tv_sec;
            long time_nanoseconds = timeEnd.tv_nsec - timeStart.tv_nsec;

            //Choosing unit of time to display and conversion of nanoseconds in milliseconds
            if (time_seconds<1){
                snprintf(buff_time, sizeof(buff_time), "| %ld ms]", time_nanoseconds/1000000);
            }
            else   {
            snprintf(buff_time, sizeof(buff_time), "| %ld s]", time_seconds);}


            // Check if the process terminated normally
            if (WIFEXITED(status)) {
                sprintf(buff_exit,"[Code exit : %d ", WEXITSTATUS(status));
                write(STDOUT_FILENO,ensea, strlen(ensea));
                write(STDOUT_FILENO, buff_exit, strlen(buff_exit));
                write(STDOUT_FILENO,buff_time, strlen(buff_time));
                write(STDOUT_FILENO, percent, strlen(percent));
            } else if (WIFSIGNALED(status)){
                sprintf(buff_exit,"[Sign : %d] ", WEXITSTATUS(status));
                write(STDOUT_FILENO,ensea, strlen(ensea));
                write(STDOUT_FILENO, buff_exit, strlen(buff_exit));
                write(STDOUT_FILENO, percent, strlen(percent));
            }
        }
    }
    return 0;
}



