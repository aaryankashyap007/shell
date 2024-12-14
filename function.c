#include "defs.h"

void function_handler(char *args[], int arg)
{
    if (strcmp(args[0], "exit") == 0)
    {
        exit_bool = false;
    }
    else if (strcmp(args[0], "hop") == 0)
    {
        hop(args, arg);
    }
    else if (strcmp(args[0], "reveal") == 0)
    {
        reveal(args, arg);
    }
    else if (strcmp(args[0], "log") == 0)
    {
        log_handler(args, arg);
    }
    else if (strcmp(args[0], "proclore") == 0)
    {
        proclore(args, arg);
    }
    else
    {
        system_commands(args);
    }
}