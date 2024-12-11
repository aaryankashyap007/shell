#include "defs.h"

bool hide;
bool details;

int compare_strings(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void print_permissions(mode_t mode)
{
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void normal(char path[])
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("");
        return;
    }
    struct dirent *entry;
    char *entries[MAX_ENTRIES];
    int count = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.' && hide)
            continue;
        if (count < MAX_ENTRIES)
        {
            entries[count] = malloc(MAX_LEN);
            if (entries[count] == NULL)
            {
                perror("Memory allocation failed");
                closedir(dir);
                return;
            }
            strncpy(entries[count], entry->d_name, MAX_LEN - 1);
            entries[count][MAX_LEN - 1] = '\0';
            count++;
        }
    }
    closedir(dir);
    qsort(entries, count, sizeof(char *), compare_strings);

    for (int i = 0; i < count; i++)
    {
        char full_path[MAX_LEN];
        snprintf(full_path, MAX_LEN, "%s/%s", path, entries[i]);

        struct stat sb;
        if (stat(full_path, &sb) == 0)
        {
            if (S_ISDIR(sb.st_mode))
            {
                printf(COLOR_BLUE "%s\n" COLOR_RESET, entries[i]);
            }
            else if (sb.st_mode & S_IXUSR)
            {
                printf(COLOR_GREEN "%s\n" COLOR_RESET, entries[i]);
            }
            else
            {
                printf(COLOR_WHITE "%s\n" COLOR_RESET, entries[i]);
            }
        }
        else
        {
            perror("stat failed");
        }

        free(entries[i]);
    }
}

void detail(char path[])
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    char *entries[MAX_ENTRIES];
    int count = 0;

    // Collect entries
    while ((entry = readdir(dir)) != NULL)
    {
        if (hide && entry->d_name[0] == '.')
            continue;

        if (count < MAX_ENTRIES)
        {
            entries[count] = malloc(MAX_LEN);
            if (entries[count] == NULL)
            {
                perror("Memory allocation failed");
                closedir(dir);
                return;
            }
            strncpy(entries[count], entry->d_name, MAX_LEN - 1);
            entries[count][MAX_LEN - 1] = '\0';
            count++;
        }
    }
    closedir(dir);

    qsort(entries, count, sizeof(char *), compare_strings);

    for (int i = 0; i < count; i++)
    {
        char full_path[MAX_LEN];
        snprintf(full_path, MAX_LEN, "%s/%s", path, entries[i]);

        struct stat sb;
        if (stat(full_path, &sb) == 0)
        {
            print_permissions(sb.st_mode);
            printf(" ");

            printf("%2ld ", (long)sb.st_nlink);

            struct passwd *pw = getpwuid(sb.st_uid);
            printf("%s ", (pw != NULL) ? pw->pw_name : "?");

            struct group *gr = getgrgid(sb.st_gid);
            printf("%s ", (gr != NULL) ? gr->gr_name : "?");

            printf("%6ld ", (long)sb.st_size);

            char time_buf[64];
            struct tm *tm = localtime(&sb.st_mtime);
            strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm);
            printf("%s ", time_buf);

            if (S_ISDIR(sb.st_mode))
                printf("%s%s%s\n", COLOR_BLUE, entries[i], COLOR_RESET);
            else if (sb.st_mode & S_IXUSR)
                printf("%s%s%s\n", COLOR_GREEN, entries[i], COLOR_RESET);
            else
                printf("%s\n", entries[i]);
        }
        else
        {
            perror("stat failed");
        }

        free(entries[i]);
    }
}

void show_files(char path[])
{
    // printf("%s\n", path);
    if (strcmp(path, ".") == 0)
    {
        // printf("1\n");
        strncpy(path, current_dir, MAX_LEN);
    }
    else if (strcmp(path, "..") == 0)
    {
        // printf("2\n");
        char temp[MAX_LEN];
        chdir("..");
        getcwd(temp, MAX_LEN);
        strncpy(path, temp, MAX_LEN);
        chdir(current_dir);
    }
    else if (strcmp(path, "~") == 0)
    {
        // printf("3\n");
        strncpy(path, home_dir, MAX_LEN);
    }
    else if (strcmp(path, "-") == 0)
    {
        // printf("4\n");
        strncpy(path, prev_dir, MAX_LEN);
    }
    else if (strncmp(path, "~/", 2) == 0)
    {
        // printf("5\n");
        char temp[MAX_LEN];
        if (snprintf(temp, MAX_LEN, "%s/%s", home_dir, path + 2) >= MAX_LEN)
        {
            fprintf(stderr, "Path is too long.\n");
            return;
        }
        strncpy(path, temp, MAX_LEN);
    }
    else if (strncmp(path, "../", 3) == 0)
    {
        // printf("6\n");
        char temp[MAX_LEN];
        if (snprintf(temp, MAX_LEN, "%s/%s", current_dir, path) >= MAX_LEN)
        {
            fprintf(stderr, "Path is too long.\n");
            return;
        }
        strncpy(path, temp, MAX_LEN);
    }

    if (!details)
    {
        normal(path);
    }
    else
    {
        detail(path);
    }
}

void handle_flags(char flag[], char path[])
{
    int l = 0, a = 0;
    for (int i = 1; i < strlen(flag); i++)
    {
        if (flag[i] == 'a')
        {
            a = 1;
        }
        else if (flag[i] == 'l')
        {
            l = 1;
        }
        else
        {
            printf("reveal: invalid option -- '%c'\n", flag[i]);
            return;
        }
    }
    if (l && !a)
    {
        hide = false;
        details = false;
    }
    else if (!l && a)
    {
        hide = true;
        details = true;
    }
    else if (l && a)
    {
        hide = false;
        details = true;
    }
    show_files(path);
}

void reveal(char *args[], int arg)
{
    if (arg == 1)
    {
        hide = true;
        details = false;
        show_files(current_dir);
    }
    else if (arg == 2)
    {
        if (args[1][0] == '-' && strlen(args[1]) > 1)
        {
            // printf("%s\n", args[1]);
            handle_flags(args[1], current_dir);
        }
        else
        {
            hide = true;
            details = false;
            show_files(args[1]);
        }
    }
    else if (arg == 3)
    {
        if (args[2][0] == '-')
        {
            int l = 0, a = 0;
            for (int i = 1; i < strlen(args[1]); i++)
            {
                if (args[1][i] == 'l')
                {
                    l = 1;
                }
                else if (args[1][i] == 'a')
                {
                    a = 1;
                }
                else
                {
                    printf("reveal: invalid option -- '%c'\n", args[1][i]);
                    return;
                }
            }
            for (int i = 1; i < strlen(args[2]); i++)
            {
                if (args[2][i] == 'l')
                {
                    l = 1;
                }
                else if (args[2][i] == 'a')
                {
                    a = 1;
                }
                else
                {
                    printf("reveal: invalid option -- '%c'\n", args[2][i]);
                    return;
                }
            }
            if (l && !a)
            {
                hide = false;
                details = false;
            }
            else if (!l && a)
            {
                hide = true;
                details = true;
            }
            else if (l && a)
            {
                hide = false;
                details = true;
            }
            show_files(home_dir);
        }
        else
        {
            handle_flags(args[1], args[2]);
        }
    }
    else if (arg == 4)
    {
        if (args[1][0] != '-' || args[2][0] != '-')
        {
            printf("Reveal: Invalid format\n");
            return;
        }
        int l = 0, a = 0;
        for (int i = 1; i < strlen(args[1]); i++)
        {
            if (args[1][i] == 'l')
            {
                l = 1;
            }
            else if (args[1][i] == 'a')
            {
                a = 1;
            }
            else
            {
                printf("reveal: invalid option -- '%c'\n", args[1][i]);
                return;
            }
        }
        for (int i = 1; i < strlen(args[2]); i++)
        {
            if (args[2][i] == 'l')
            {
                l = 1;
            }
            else if (args[2][i] == 'a')
            {
                a = 1;
            }
            else
            {
                printf("reveal: invalid option -- '%c'\n", args[2][i]);
                return;
            }
        }
        if (l && !a)
        {
            hide = false;
            details = false;
        }
        else if (!l && a)
        {
            hide = true;
            details = true;
        }
        else if (l && a)
        {
            hide = false;
            details = true;
        }
        show_files(args[3]);
    }
    else
    {
        printf("Reveal: Too many inputs\nDefault: reveal <flags> <path/name>\n");
    }
}