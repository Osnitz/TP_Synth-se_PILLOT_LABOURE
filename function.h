// Include

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
// Define

#define welcome_msg "Bienvenue dans le Shell ENSEA.\n"
#define exit_instruction "Pour quitter, tapez 'exit'. \n"
#define msg_enseash "enseash % "
#define exit_msg "Bye Bye...\n"
#define error_msg "invalid command\n"
#define empty_msg "empty command\n"
#define ensea "ensea"
#define percent " % "
#define MAXSIZE 1024

// Function
void welcome();
void display_execTime_exitCode_prompt(int status, char *buff_exit,char *buff_time,struct timespec start, struct timespec end);
void exec_complex_command(char command[]);
void exec_complex_command_redirection(char command[]);