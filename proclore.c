#include "defs.h"

void proclore_print(const char *pid)
{
    char path[256], buffer[1024], state;
    FILE *file;
    int group_id;
    unsigned long vm_size;
    char exe_path[256];

    if (pid == NULL)
    {
        snprintf(path, sizeof(path), "/proc/%d/stat", getpid());
        pid = malloc(16);
        if (pid == NULL)
        {
            perror("Memory allocation failed");
            return;
        }
        snprintf((char *)pid, 16, "%d", getpid());
    }

    file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error opening stat file");
        if (pid != NULL)
            free((void *)pid);
        return;
    }

    if (fscanf(file, "%*d %*s %c %*d %*d %d", &state, &group_id) != 2)
    {
        perror("Error reading stat file");
        fclose(file);
        if (pid != NULL)
            free((void *)pid);
        return;
    }
    fclose(file);

    snprintf(path, sizeof(path), "/proc/%s/statm", pid);
    file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error opening statm file");
        if (pid != NULL)
            free((void *)pid);
        return;
    }

    if (fscanf(file, "%lu", &vm_size) != 1)
    {
        perror("Error reading statm file");
        fclose(file);
        if (pid != NULL)
            free((void *)pid);
        return;
    }
    fclose(file);

    vm_size *= sysconf(_SC_PAGESIZE) / 1024;

    snprintf(path, sizeof(path), "/proc/%s/exe", pid);
    ssize_t exe_len = readlink(path, exe_path, sizeof(exe_path) - 1);
    if (exe_len == -1)
    {
        perror("Error reading exe path");
        strncpy(exe_path, "Unknown", sizeof(exe_path));
    }
    else
    {
        exe_path[exe_len] = '\0';
    }

    printf("PID: %s\n", pid);
    printf("Process Status: %c\n", state);
    printf("Process Group: %d\n", group_id);
    printf("Virtual Memory: %lu KB\n", vm_size);
    printf("Executable Path: %s\n", exe_path);

    if (pid != NULL)
        free((void *)pid);
}

void proclore(char*args[], int arg)
{
    if (arg > 2)
    {
        fprintf(stderr, "Usage: %s [pid]\n", args[0]);
        return;
    }

    const char *pid = (arg == 2) ? args[1] : NULL;
    proclore_print(pid);
}