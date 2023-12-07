#include "function.h"

/*void display_execTime_exitCode_prompt(int status, char *buff_exit,char *buff_time,struct timespec start, struct timespec end){
    //Computing the program's duration
    long time_seconds = end.tv_sec - start.tv_sec;
    long time_nanoseconds = end.tv_nsec - start.tv_nsec;

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
}*/

/*void exec_complex_command(char command[]){
    char *args[MAXSIZE];  // Array to store arguments

    char *token = strtok(command, " "); // Initialize token inside the loop of  child process;
    int argument_index = 0; // Reset argument_index

    while (token != NULL) {
        args[argument_index++] = token;
        token = strtok(NULL, " ");
    }
    args[argument_index] = NULL;  // The last element must be NULL for execvp

    // Execute the command with its arguments
    execvp(args[0], args);
}*/


int main(void) {
    char command[MAXSIZE];
    int numberOfChar;
    int status;
    char buff_exit[MAXSIZE];
    char buff_time[MAXSIZE];
    char *token;
    char *args[MAXSIZE];  // Array to store arguments
    int argument_index;

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

        clock_gettime(CLOCK_MONOTONIC, &timeStart); // Capture start time
        pid_t ret = fork();

        if (ret == 0) {// Child process code

            exec_complex_command(command);
            // If execvp fails, print an error message
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(EXIT_FAILURE);
        } else {
            // Parent process code
            wait(&status);
            clock_gettime(CLOCK_MONOTONIC, &timeEnd);

            display_execTime_exitCode_prompt(status,buff_exit,buff_time, timeStart,timeEnd);
        }
    }
    return 0;
}




