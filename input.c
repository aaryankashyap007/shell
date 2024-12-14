#include "defs.h"

char last_input[MAX_LEN];

int count_lines_in_file()
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return -1;
    }

    int line_count = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            line_count++;
        }
    }

    fclose(file);
    return line_count;
}

void truncate_and_append(char *args[], int arg)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open file for reading");
        return;
    }

    FILE *temp = tmpfile();
    if (temp == NULL)
    {
        perror("Failed to create temporary file");
        fclose(file);
        return;
    }

    char buffer[MAX_LEN];
    int skip_first = 1;

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (skip_first)
        {
            skip_first = 0;
            continue;
        }
        fputs(buffer, temp);
    }

    fclose(file);

    for (int i = 0; i < arg; i++)
    {
        fprintf(temp, "%s ", args[i]);
    }

    fprintf(temp, "\n");

    file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Failed to open file for writing");
        fclose(temp);
        return;
    }

    rewind(temp);
    while (fgets(buffer, sizeof(buffer), temp))
    {
        fputs(buffer, file);
    }

    fclose(temp);
    fclose(file);
}

void append_to_file(char *args[], int arg)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < arg; i++)
    {
        fprintf(file, "%s ", args[i]);
    }

    fprintf(file, "\n");
    fclose(file);
}

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
    char temp[MAX_LEN];
    strncpy(temp, "\0", MAX_LEN);
    for (int j = 0; j < i; j++)
    {
        strcat(temp, args[j]);
        if (j < i - 1)
        {
            strcat(temp, " ");
        }
    }
    if (strcmp(lastcommand, temp) != 0 && strcmp(args[0], "log") != 0)
    {
        if (count_lines_in_file() == MAX_FILES)
        {
            truncate_and_append(args, i);
        }
        else
        {
            append_to_file(args, i);
        }
    }

    strncpy(lastcommand, "\0", MAX_LEN);
    for (int j = 0; j < i; j++)
    {
        strcat(lastcommand, args[j]);
        if (j < i - 1)
        {
            strcat(lastcommand, " ");
        }
    }

    strncpy(last_input, args[0], MAX_LEN);
    start_time = times(NULL);
    function_handler(args, i);
    end_time = times(NULL);
    for (int i = 0; i < MAX_ARGS; i++)
    {
        if (args[i] != NULL)
        {
            free(args[i]);
            args[i] = NULL;
        }
    }
}

void multiple_function_handler(char input[])
{
    char *token;
    char *saveptr;
    token = strtok_r(input, ";", &saveptr);
    while (token != NULL)
    {
        input_splitter(token);
        token = strtok_r(NULL, ";", &saveptr);
    }
}

void input_handler(char input[])
{
    char *copy = strdup(input);
    if (copy == NULL)
    {
        perror("Memory allocation failed");
        return;
    }

    char *chunks[1024];
    int count = 0;

    char *token = strtok(copy, "&");
    while (token != NULL)
    {
        chunks[count++] = token;
        token = strtok(NULL, "&");
    }

    for (int i = 0; i < count - 1; i++)
    {
        // printf("background\n");
        is_bg = true;
        multiple_function_handler(chunks[i]);
    }

    if (count > 0)
    {
        // printf("foreground\n");
        is_bg = false;
        multiple_function_handler(chunks[count - 1]);
    }

    free(copy);
}