// Include

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// Define

#define welcome_msg "Bienvenue dans le Shell ENSEA.\n"
#define exit_instruction "Pour quitter, tapez 'exit'. \n"
#define msg_enseash "enseash % "
#define exit_msg "Bye Bye...\n"
#define error_msg "invalid command\n"
#define empty_msg "empty command\n"
#define ensea "ensea"
#define percent "% "
#define MAXSIZE 1024

// Function
void welcome();