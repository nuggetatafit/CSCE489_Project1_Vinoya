/*************************************************************************************
 * myshell - Nate Vinoya's code for Project 1 of CSCE 489 
 *************************************************************************************/

// standard c libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
// the header file for shell functions
#include "shellfuncts.h"
	
int main() { // load configuration files
	cmd_loop(); // run command loop
	return EXIT_SUCCESS; // return success
}
