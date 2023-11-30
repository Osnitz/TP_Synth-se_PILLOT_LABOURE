
// includes
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// Define

#define welcome_msg "Welcom in the ENSEA Shell .\nTo leave, write 'exit'. \n"
#define msg_enseash "enseash% \n"

// functions
void welcome();
