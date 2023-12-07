#include "function.h"

/*void welcome() {
    write(STDOUT_FILENO, welcome_msg, strlen(welcome_msg));
    write(STDOUT_FILENO, exit_instruction, strlen(exit_instruction));
    write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
}*/

int main(void) {
    char command[MAXSIZE];
    int numberOfChar;
    welcome();

    while (1) {
        // Read user input
        numberOfChar = read(STDIN_FILENO, command, MAXSIZE);
        if (numberOfChar == -1) {
            perror("read");
            return 1;
        }
        command[numberOfChar - 1] = '\0'; //we replace "\n" by "\0" to delimit the command

        // Exit if the user types 'exit'
        if (strcmp(command, "exit") == 0) {
            write(STDOUT_FILENO, exit_msg, strlen(exit_msg));
            break;
        } else {
            // Add error checking for the write system call
            if (write(STDOUT_FILENO, error_msg, strlen(error_msg)) == -1) {
                perror("write");
                return 1;
            }
            if (write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash)) == -1) {
                perror("write");
                return 1;
            }
        }
    }
    return 0;
}
