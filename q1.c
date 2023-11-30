#include "function.h"

void welcome() {
    write(STDOUT_FILENO, welcome_msg, strlen(welcome_msg));
    write(STDOUT_FILENO, msg_enseash, strlen(msg_enseash));
}

int main (void){
    welcome();
}