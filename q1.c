#include "function.h"

void welcome() {
    write(STDOUT_FILENO, welcome_msg, strlen(welcome_msg));
    write(STDOUT_FILENO, exit_instruction, strlen(exit_instruction));
    write(STDOUT_FILENO, msg_enseash , strlen(msg_enseash));
}

int main(void) {
    char command[MAXSIZE];
    int numberOfChar;
    int status;

    while (1) {
        welcome();

        // Read user input
        numberOfChar = read(STDIN_FILENO, command, MAXSIZE);
        command[numberOfChar - 1] = '\0';

        // Exit if the user types 'exit'
        if (strcmp(command, "exit") == 0) {
            write(STDOUT_FILENO, exit_msg, strlen(exit_msg));
            break;
        }
    return 0;
    }
}
