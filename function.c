#include "function.h"

void welcome() {
    write(STDOUT_FILENO, welcome_msg, strlen(welcome_msg));
    write(STDOUT_FILENO, exit_instruction, strlen(exit_instruction));
    write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
}

void display_execTime_exitCode_prompt(int status, char *buff_exit,char *buff_time,struct timespec start, struct timespec end){
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
}

void exec_complex_command(char command[]){
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
}
