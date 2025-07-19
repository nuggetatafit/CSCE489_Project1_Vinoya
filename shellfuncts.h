/*************************************************************************************
 * shellfuncts.h - header file for shell functions
 *
 *************************************************************************************/

#ifndef SHELLFUNCTS_H
#define SHELLFUNCTS_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

void cmd_loop(void);
void trim_whitespace(char *str);
void cmd_create(char *filename);
void cmd_update(char *filename, int count, char *text);
void cmd_list(char *filename);
void cmd_dir(void);

#endif
