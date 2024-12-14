#ifndef DEFS_H
#define DEFS_H

#define MAX_ENTRIES 1024
#define MAX_LEN 256
#define MAX_FILES 15
#define MAX_ARGS 16

#define COLOR_RESET "\033[0m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_WHITE "\033[1;37m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <sys/times.h>

extern bool exit_bool;
extern char filename[MAX_LEN];
extern char lastcommand[MAX_LEN];
extern clock_t start_time, end_time;
extern double time_taken;
extern char last_input[MAX_LEN];
extern long clock_ticks;

// print
void start();
void print();
void getdir();
extern char username[MAX_LEN];
extern char systemname[MAX_LEN];
extern char home_dir[MAX_LEN];
extern char current_dir[MAX_LEN];
extern char display_dir[MAX_LEN];
extern char prev_dir[MAX_LEN];

// input
void input_handler(char input[]);
void input_splitter(char commands[]);
void truncate_and_append(char *args[], int arg);
void append_to_file(char *args[], int arg);
int count_lines_in_file();

// function
void function_handler(char *args[], int arg);

// hop
void hop(char *args[], int arg);
void hop_directory(char args[]);

// reveal
extern bool hide;
extern bool details;
void reveal(char *args[], int arg);
void show_files(char path[]);
void detail(char path[]);
void normal(char path[]);
void handle_flags(char flag[], char path[]);
int compare_strings(const void *a, const void *b);

// log
void log_handler(char *args[], int arg);

// system commands
void system_commands(char *args[]);

// proclore
void proclore_print(const char *pid);
void proclore(char*args[], int arg);

#endif