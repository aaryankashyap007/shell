#ifndef DEFS_H
#define DEFS_H

#define MAX_LEN 64
#define MAX_ARGS 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/utsname.h>

extern bool exit_bool;

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
void input_splitter(char commands[]);

// function
void function_handler(char *args[], int arg);

#endif