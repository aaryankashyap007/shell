#include "defs.h"

void hop_directory(char args[])
{
    if (strcmp(args, "~") == 0)
    {
        // printf("1\n");
        strncpy(prev_dir, current_dir, MAX_LEN);
        chdir(home_dir);
        strncpy(current_dir, home_dir, MAX_LEN);
        printf("%s\n", home_dir);
        getdir();
    }
    else if (strcmp(args, "-") == 0)
    {
        // printf("2\n");
        chdir(prev_dir);
        char temp[MAX_LEN];
        strncpy(temp, prev_dir, MAX_LEN);
        strncpy(prev_dir, current_dir, MAX_LEN);
        strncpy(current_dir, temp, MAX_LEN);
        printf("%s\n", current_dir);
        getdir();
    }
    else if (strcmp(args, ".") == 0)
    {
        // printf("3\n");
        strncpy(prev_dir, current_dir, MAX_LEN);
        printf("%s\n", current_dir);
    }
    else if (strcmp(args, "..") == 0)
    {
        // printf("4\n");
        strncpy(prev_dir, current_dir, MAX_LEN);
        chdir("..");
        getcwd(current_dir, MAX_LEN);
        printf("%s\n", current_dir);
        getdir();
    }
    else
    {
        // printf("5\n");
        char temp[MAX_LEN];
        strncpy(temp, current_dir, MAX_LEN);
        strcat(temp, "/");
        strcat(temp, args);

        if (chdir(temp) == 0)
        {
            strncpy(prev_dir, current_dir, MAX_LEN);
            strncpy(current_dir, temp, MAX_LEN);
            printf("%s\n", current_dir);
            getdir();
        }
        else
        {
            perror("hop failed");
        }
    }
}

void hop(char *args[], int arg)
{
    if (arg == 1)
    {
        // printf("0\n");
        strncpy(prev_dir, current_dir, MAX_LEN);
        chdir(home_dir);
        strncpy(current_dir, home_dir, MAX_LEN);
        printf("%s\n", home_dir);
        getdir();
    }
    else
    {
        for (int i = 1; i < arg; i++)
        {
            // printf("%s\n", args[i]);
            hop_directory(args[i]);
        }
    }
}