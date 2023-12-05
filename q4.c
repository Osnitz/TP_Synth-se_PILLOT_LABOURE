#include "function.h"

void welcome() {
    write(STDOUT_FILENO, welcome_msg, strlen(welcome_msg));
    write(STDOUT_FILENO, exit_instruction, strlen(exit_instruction));
    write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
}

int main(void) {
    char command[MAXSIZE];
    int numberOfChar;
    int status;
    char buff[MAXSIZE];

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

            // Check if the process terminated normally
            if (WIFEXITED(status)) {
                sprintf(buff,"[Code exit : %d] ", WEXITSTATUS(status));
                write(STDOUT_FILENO,ensea, strlen(ensea));
                write(STDOUT_FILENO, buff, strlen(buff));
                write(STDOUT_FILENO, percent, strlen(percent));
            } else if (WIFSIGNALED(status)){
                sprintf(buff,"[Sign : %d] ", WEXITSTATUS(status));
                write(STDOUT_FILENO,ensea, strlen(ensea));
                write(STDOUT_FILENO, buff, strlen(buff));
                write(STDOUT_FILENO, percent, strlen(percent));
            }
        }
    }

    return 0;
}



