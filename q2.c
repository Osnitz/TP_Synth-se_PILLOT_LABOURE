#include "function.h"

void welcome() {
    write(STDOUT_FILENO, welcome_msg, strlen(welcome_msg));
    write(STDOUT_FILENO, exit_instruction, strlen(exit_instruction));
    write(STDOUT_FILENO, msg_enseash , strlen(msg_enseash));
}

int main(void) {
    char command[MAXSIZE];
    int numberOfChar;
    char *fortune_agrs[] ={"fortune",NULL};

    int status;
    pid_t ret = fork();
    if (ret != 0){
        welcome();
    }
    while (1) {
        // Read user input
        numberOfChar = read(STDIN_FILENO, command, MAXSIZE);
        command[numberOfChar - 1] = '\0';

        // Exit if the user types 'exit'
        if (strcmp(command, "exit") == 0) {
            write(STDOUT_FILENO, exit_msg, strlen(exit_msg));
            break;
        }
        if((strcmp(command, "fortune") == 0))if (ret== 0){
                if (ret== 0){
                //   execvp(fortune_agrs[0],fortune_agrs);
                execlp(command,command,NULL);
                }
                else{

                }
        }

        else{
            wait(&status);
            write(STDOUT_FILENO,error_msg, strlen(error_msg));
            write(STDOUT_FILENO, msg_enseash , strlen(msg_enseash));
        }
    }
    return 0;
}
