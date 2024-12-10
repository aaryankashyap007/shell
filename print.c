#include "defs.h"

char username[MAX_LEN];
char systemname[MAX_LEN];
char home_dir[MAX_LEN];
char current_dir[MAX_LEN];
char display_dir[MAX_LEN];
char prev_dir[MAX_LEN];

void getdir()
{
    getcwd(display_dir, MAX_LEN);
    if (strcmp(home_dir, display_dir) == 0)
    {
        strcpy(display_dir, "~");
    }
    else
    {
        size_t home_len = strlen(home_dir);
        if (strncmp(home_dir, display_dir, home_len) == 0 && display_dir[home_len] == '/')
        {
            char temp[MAX_LEN];
            snprintf(temp, MAX_LEN, "~%s", display_dir + home_len);
            strncpy(display_dir, temp, MAX_LEN - 1);
            display_dir[MAX_LEN - 1] = '\0';
        }
    }
}

void start()
{
    const char *user = getenv("USER");
    if (user)
    {
        strncpy(username, user, MAX_LEN - 1);
        username[MAX_LEN - 1] = '\0';
    }
    else
    {
        perror("username");
        exit(1);
    }

    struct utsname system_info;

    if (uname(&system_info) == 0)
    {
        strncpy(systemname, system_info.nodename, MAX_LEN - 1);
    }
    else
    {
        perror("uname");
        exit(1);
    }

    getcwd(current_dir, MAX_LEN);
    getcwd(home_dir, MAX_LEN);
    getcwd(prev_dir, MAX_LEN);

    getdir();
}

void print()
{
    const char *green = "\033[1;32m";
    const char *blue = "\033[1;34m";
    const char *reset = "\033[0m";
    printf("%s%s@%s%s:%s%s%s$ ", green, username, systemname, reset, blue, display_dir, reset);
    char input[MAX_LEN];
    fgets(input, sizeof(input), stdin);
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
    }
    input_handler(input);
}