#include "defs.h"

void input_splitter(char commands[])
{
    char *args[MAX_ARGS];
    char *token;
    char *saveptr;
    int i = 0;

    token = strtok_r(commands, " ", &saveptr);
    while (token != NULL && i < MAX_ARGS)
    {
        args[i] = malloc(MAX_LEN * sizeof(char));
        if (args[i] == NULL)
        {
            perror("Memory allocation failed");
            exit(1);
        }
        strncpy(args[i], token, MAX_LEN - 1);
        args[i][MAX_LEN - 1] = '\0';
        token = strtok_r(NULL, " ", &saveptr);
        i++;
    }
    for (; i < MAX_ARGS; i++)
    {
        args[i] = NULL;
    }
    i = 0;
    while (args[i] != NULL)
    {
        // printf("%s\n", args[i]);
        i++;
    }
    // printf("%d\n", i);
    function_handler(args, i);
    for (int i = 0; i < MAX_ARGS; i++)
    {
        if (args[i] != NULL)
        {
            free(args[i]);
            args[i] = NULL;
        }
    }
}

void input_handler(char input[])
{
    char *token;
    char *saveptr;
    token = strtok_r(input, "&;", &saveptr);
    while (token != NULL)
    {
        input_splitter(token);
        token = strtok_r(NULL, "&;", &saveptr);
    }
}
