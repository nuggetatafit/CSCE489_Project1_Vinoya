/*************************************************************************************
 * shellfuncts.c - code definitions for your functions
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
#include <stddef.h>
// the header file for shell functions
#include "shellfuncts.h"

// define constants and buffers
#define max_line 256 // maximum length of input line
char line[max_line]; // buffer to hold the input line
char filename_arg[max_line]; // buffer to hold the filename argument
char text_arg[max_line]; // buffer to hold the text argument
int count_arg; // variable to hold the count argument
/*************************************************************************************
 * trim_whitespace - trims whitespace from the end of a string (for parsing)
 *************************************************************************************/
void trim_whitespace(char *str) { 
    int i = strlen(str) - 1; // start from the end of the string
    while (i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')) { // check for whitespace characters
        str[i--] = '\0'; // replace with null terminator
    }
}
/*************************************************************************************
 * cmd_loop - main command loop for the shell
 *************************************************************************************/
void cmd_loop(void) {
    // print boot up message for shell and pid
    printf("Welcome to Nate Vinoya's nutshell!!! (Parent Process ID: %d)\n", getpid());
    while (1) { // infinite loop until halt command is entered
        printf("nutshell> "); // prompt user for input
        fflush(stdout); // ensure prompt is printed before reading input
        // reading the input line
        if (fgets(line, max_line, stdin) == NULL) { // handle end-of-file or error
            printf("\nGoodbye!!\n"); // terminate shell on end-of file
            break; // exit the loop
        }

        // background execution handling
        // check if the command should run in the background
        // by looking for an ampersand '&' at the end of the line
        int background = 0; // flag to indicate background execution
        char *ampersand = strchr(line, '&'); // find the ampersand character
        if (ampersand != NULL) { // if ampersand is found
            background = 1; // set background flag
            *ampersand = '\0'; // remove the ampersand from the command
        }
        trim_whitespace(line); // trim whitespace from the end of the line
        if (line[0] == '\0') continue; // skip empty lines

        // use scanf to parse the command and its arguments
        pid_t pid = -1; // use -1 to for no forked process yet
        if (strcmp(line, "halt") == 0) { // check for halt command
            printf("Goodbye!!\n"); // print goodbye message
            break; // exit the loop
        } 
        else if (sscanf(line, "update %s %d \"%[^\"]\"", filename_arg, &count_arg, text_arg) == 3) { // check for update command
            if (count_arg <= 0) { // check if count is a positive integer
                fprintf(stderr, "Error: Count must be a positive integer\n"); // print error message
                continue; // skip to next iteration
            }
            pid = fork(); // fork a new process
            if (pid == 0) cmd_update(filename_arg, count_arg, text_arg); // child process executes cmd_update
        } 
        else if (sscanf(line, "create %s", filename_arg) == 1) { // check for create command
            pid = fork(); // fork a new process
            if (pid == 0) cmd_create(filename_arg); // child process executes cmd_create
        } 
        else if (sscanf(line, "list %s", filename_arg) == 1) { // check for list command
            pid = fork(); // fork a new process
            if (pid == 0) cmd_list(filename_arg); // child process executes cmd_list
        } 
        else if (strcmp(line, "dir") == 0) { // check for dir command
            pid = fork(); // fork a new process
            if (pid == 0) cmd_dir(); // child process executes cmd_dir
        } 
        else { // if command does not match any known commands
            fprintf(stderr, "Error: Unknown command or invalid syntax: '%s'\n", line); // print error message
        }

        // parent process handling
        if (pid > 0) { // if fork was successful
            if (!background) { // if not running in background
                waitpid(pid, NULL, 0); // wait for the child process to finish
                printf("Child process %d finished.\n", pid); // print child process completion message
            } else { // if running in background
                printf("Command running in background (PID: %d)\n", pid); // print background child process message
            }
        } 
        else if (pid < 0) { // fork failed
            fprintf(stderr, "Error: Fork failed\n"); // print error message
        }
    }
}
/*************************************************************************************
 * cmd_create - creates the file
 *
 *************************************************************************************/
// create command implementation
void cmd_create(char *filename) {
    FILE *fp = fopen(filename, "r"); // try to open file in read mode to check if it exists
    if (fp != NULL) { // if file exists
        fprintf(stderr, "Error: File already exists - %s\n", strerror(errno));
        fclose(fp); // close the file if it exists
        exit(1); // exit with error
    }
    fp = fopen(filename, "w"); // try to create file in write mode
    if (fp == NULL) { // if file creation failed
        fprintf(stderr, "Error creating file - %s\n", strerror(errno));
        exit(1); // exit with error
    }
    fclose(fp); // close the file after creation
    exit(0); // exit child process successfully
}
/*************************************************************************************
 * cmd_update - updates the file
 *
 *************************************************************************************/
// update command implementation
void cmd_update(char *filename, int count, char *text) {
    FILE *fp = fopen(filename, "a"); // open file in append mode
    if (fp == NULL) { // if file does not exist or cannot be opened
        fprintf(stderr, "Error opening file for append - %s\n", strerror(errno));
        exit(1); // exit with error
    }
    for (int i = 0; i < count; i++) { // loop to write text to file x amount of times
        fprintf(fp, "%s\n", text); // write text to file
        fflush(fp); // ensure text is written immediately
        sleep(strlen(text) / 5); // sleep for a duration based on text length
    }
    fclose(fp); // close the file after writing
    printf("Update completed (PID: %d)\n", getpid()); // print completion message with child pid
    exit(0); // exit child process successfully
}
/*************************************************************************************
 * cmd_list - lists what is within the file
 *
 *************************************************************************************/
void cmd_list(char *filename) { // check if file exists
    FILE *fp = fopen(filename, "r"); // open file in read mode
    if (fp == NULL) { // if file does not exist or cannot be opened
        fprintf(stderr, "Error: File does not exist - %s\n", strerror(errno));
        exit(1); // terminate the child process with error
    }
    fclose(fp); // close the file after checking existence

    // note: the following algorithm was written in the instructions but doesn't
    // actually do anything relevant so I kept it out:
    // construct cat command using snprintf(command, bufsize, "cat %s", <name>)

    execl("/bin/cat", "cat", filename, NULL); // execute cat command to display file contents
    exit(0); // exit without error
}
/*************************************************************************************
 * cmd_dir - essentially lists directory commands
 *
 *************************************************************************************/
void cmd_dir() { // use execl to run ls command
    execl("/bin/ls", "ls", NULL); // execute ls command to list directory contents
    exit(0); // exit without error
}
