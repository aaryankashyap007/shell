#include "defs.h"

int is_natural_number(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0;

    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }

    return 1;
}

void log_handler(char *args[], int arg)
{
    if (arg == 1)
    {
        FILE *file = fopen(filename, "r");
        if (file == NULL)
        {
            perror("Unable to open file");
            return;
        }

        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            printf("%s", buffer);
        }

        fclose(file);
    }
    else if (arg == 2)
    {
        if (strcmp(args[1], "purge") == 0)
        {
            FILE *file = fopen(filename, "w");
            if (file == NULL)
            {
                perror("Unable to open file");
                return;
            }

            fclose(file);
        }
        else
        {
            printf("log: invalid command\n");
            return;
        }
    }
    else if (arg == 3)
    {
        if (strcmp(args[1], "execute") == 0)
        {
            if (is_natural_number(args[2]))
            {
                if (atoi(args[2]) <= count_lines_in_file())
                {
                    int target_command = count_lines_in_file() - atoi(args[2]) + 1;
                    FILE *file = fopen(filename, "r");
                    if (file == NULL)
                    {
                        perror("Unable to open file");
                        return;
                    }

                    char buffer[MAX_LEN];
                    int current_line = 0;
                    char *result = NULL;

                    while (fgets(buffer, MAX_LEN, file) != NULL)
                    {
                        current_line++;
                        if (current_line == target_command)
                        {
                            result = strdup(buffer);
                            if (result == NULL)
                            {
                                perror("Memory allocation failed");
                                return;
                            }
                            break;
                        }
                    }

                    fclose(file);

                    if (result != NULL)
                    {
                        size_t len = strlen(result);
                        if (len > 0 && result[len - 1] == '\n')
                        {
                            result[len - 1] = '\0';
                        }
                    }
                    char *temp_args[MAX_ARGS];
                    char *token;
                    char *saveptr;
                    int i = 0;

                    token = strtok_r(result, " ", &saveptr);
                    while (token != NULL && i < MAX_ARGS)
                    {
                        temp_args[i] = malloc(MAX_LEN * sizeof(char));
                        if (temp_args[i] == NULL)
                        {
                            perror("Memory allocation failed");
                            exit(1);
                        }
                        strncpy(temp_args[i], token, MAX_LEN - 1);
                        temp_args[i][MAX_LEN - 1] = '\0';
                        token = strtok_r(NULL, " ", &saveptr);
                        i++;
                    }
                    for (; i < MAX_ARGS; i++)
                    {
                        temp_args[i] = NULL;
                    }
                    i = 0;
                    while (temp_args[i] != NULL)
                    {
                        i++;
                    }
                    function_handler(temp_args, i);
                }
                else
                {
                    printf("log: command out of bound\n");
                    return;
                }
            }
            else
            {
                printf("log: invalid command\n");
                return;
            }
        }
        else
        {
            printf("log: invalid command\n");
            return;
        }
    }
}