#include "defs.h"

char username[MAX_LEN];
char systemname[MAX_LEN];
char home_dir[MAX_LEN];
char current_dir[MAX_LEN];
char display_dir[MAX_LEN];
char prev_dir[MAX_LEN];
char filename[MAX_LEN];
char lastcommand[MAX_LEN];
double time_taken;
long clock_ticks;
clock_t start_time, end_time;
bool is_bg;

char *get_last_line()
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Unable to open file");
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        perror("Error seeking to the end of the file");
        fclose(file);
        return NULL;
    }

    long file_size = ftell(file);
    if (file_size == -1)
    {
        perror("Error getting file size");
        fclose(file);
        return NULL;
    }

    long pos = file_size - 1;
    int ch;
    while (pos >= 0)
    {
        fseek(file, pos, SEEK_SET);
        ch = fgetc(file);
        if (ch == '\n' && pos != file_size - 1)
        {
            pos++;
            break;
        }
        pos--;
    }

    if (pos < 0)
        pos = 0;

    fseek(file, pos, SEEK_SET);
    size_t buffer_size = 1024;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL)
    {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    if (fgets(buffer, buffer_size, file) != NULL)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
    }
    else
    {
        free(buffer);
        buffer = NULL;
    }

    fclose(file);
    return buffer;
}

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

    char temp_filename[MAX_LEN];
    strncpy(temp_filename, "log_commands.txt", MAX_LEN);
    FILE *file = fopen(temp_filename, "a");
    if (file == NULL)
    {
        perror("Failed to create file");
        return;
    }
    fclose(file);
    strcat(filename, home_dir);
    strcat(filename, "/");
    strcat(filename, temp_filename);
    if (get_last_line() == NULL)
    {
        strncpy(lastcommand, "\0", MAX_LEN);
    }
    else
    {
        char temp[MAX_LEN];
        strncpy(temp, get_last_line(), MAX_LEN);
        temp[strlen(temp) - 1] = '\0';
        strncpy(lastcommand, temp, MAX_LEN);
    }
    clock_ticks = sysconf(_SC_CLK_TCK);
    if (clock_ticks == -1)
    {
        perror("sysconf failed");
        return;
    }
    is_bg = false;
}

void print()
{
    // const char *green = "\033[1;32m";
    // const char *blue = "\033[1;34m";
    // const char *reset = "\033[0m";
    time_taken = ((double)(end_time - start_time)) / clock_ticks;
    if (time_taken > 2)
    {
        printf("%s%s@%s%s:%s%s%s %s : %ds$ ", COLOR_GREEN, username, systemname, COLOR_RESET, COLOR_BLUE, display_dir, COLOR_RESET, last_input, (int)time_taken);
    }
    else
    {
        printf("%s%s@%s%s:%s%s%s$ ", COLOR_GREEN, username, systemname, COLOR_RESET, COLOR_BLUE, display_dir, COLOR_RESET);
    }
    char input[MAX_LEN];
    fgets(input, sizeof(input), stdin);
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
    }
    input_handler(input);
}