#include "defs.h"

void function_handler(char *args[], int arg)
{
    if (strcmp(args[0], "exit") == 0)
    {
        exit_bool = false;
    }
}