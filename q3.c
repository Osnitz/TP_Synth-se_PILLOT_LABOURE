#include "function.h"

int main(void) {
    char command[MAXSIZE];
    int numberOfChar;
    int status;

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

        // exit when user types ctrl+D
        if (numberOfChar == 0) {
            if (write(STDOUT_FILENO, "\nGoodbye!\n", strlen("\nGoodbye!\n")) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            break;
        }

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

            // Check if the process terminated normally
            if (WIFEXITED(status)) {
                if (write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash)) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            } else {
                if (write(STDOUT_FILENO, "Child process did not terminate normally.\n", strlen("Child process did not terminate normally.\n")) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    return 0;
}
