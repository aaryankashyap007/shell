#include "defs.h"

void system_commands(char *args[])
{
    pid_t pid = fork();

    if (pid < 0)
    {
        // Fork failed
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            printf("Invalid command\n");
            exit(1);
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);

        if (!WIFEXITED(status))
        {
            printf("Command execution failed\n");
        }
    }
}