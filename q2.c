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

    welcome();

    while (1) {
        // Read user input
        numberOfChar = read(STDIN_FILENO, command, MAXSIZE);
        command[numberOfChar - 1] = '\0';

        if (strlen(command) == 0) {
            write(STDOUT_FILENO, empty_msg, strlen(empty_msg));
            write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
            continue;
        }
        // Exit if the user types 'exit'
        if (strcmp(command, "exit") == 0) {
            write(STDOUT_FILENO, exit_msg, strlen(exit_msg));
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
                write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
            } else {
                write(STDOUT_FILENO, "Child process did not terminate normally.\n", strlen("Child process did not terminate normally.\n"));
            }
        }
    }
    return 0;
}
