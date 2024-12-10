#ifndef DEFS_H
#define DEFS_H

#define MAX_LEN 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

// print
void start();
void print();
void getdir();
extern char username[MAX_LEN];
extern char systemname[MAX_LEN];
extern char home_dir[MAX_LEN];
extern char current_dir[MAX_LEN];
extern char display_dir[MAX_LEN];

// input
void input_handler(char input[]);

#endif