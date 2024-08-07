#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void d2b(int x, char ans[])
{
    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
        {
            ans[2 - i] = (x % 2) ? 'x' : '-';
        }
        else if (i == 1)
        {
            ans[2 - i] = (x % 2) ? 'w' : '-';
        }
        else
        {
            ans[2 - i] = (x % 2) ? 'r' : '-';
        }
        x /= 2;
    }
    ans[3] = '\0';
}

void list_dir(char *path, int flag)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    while (entry = readdir(dir))
    {
        if (strcmp(entry->d_name, ".") == 0)
            continue;

        if (strcmp(entry->d_name, "..") == 0)
            continue;

        if (flag)
        {
            // 带-l选项
            char symbol[100] = "/";
            char *current_path = strcat(symbol, entry->d_name);
            char current_s[100];
            strcpy(current_s, current_path);
            char path_s[100];
            strcpy(path_s, path);
            char *file_path = strcat(path_s, current_s); // 拼接出当前文件的路径

            struct stat attr;
            stat(file_path, &attr);

            int mode = (&attr)->st_mode & 0x1ff;
            int u = (mode >> 6) & 7;
            int g = (mode >> 3) & 7;
            int o = mode & 7;

            // 下面将三个标志转换为二进制
            char us[4];
            char gs[4];
            char os[4];
            d2b(u, us);
            d2b(g, gs);
            d2b(o, os);

            printf("%s%s%s ", us, gs, os);
        }

        if (entry->d_type == DT_DIR)
            printf("%s\n", entry->d_name);

        if (entry->d_type == DT_REG)
            printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    char *path;   // 路径
    int flag = 0; // 是否有-l的标志

    if (argc == 1)
    {
        path = "./";
        flag = 0;
    }
    else if (argc == 2)
    {
        if (!(strcmp(argv[1], "-l")))
        {
            path = "./";
            flag = 1;
        }
        else
        {
            path = "";
            flag = 0;
            char path_s[100];
            strcpy(path_s, path);
            path = strcat(path_s, argv[1]);
        }
    }
    else if (argc == 3)
    {
        path = "";
        flag = 1;
        char path_s[100];
        strcpy(path_s, path);
        if (!(strcmp(argv[1], "-l")))
            path = strcat(path_s, argv[2]);
        else if (!(strcmp(argv[2], "-l")))
            path = strcat(path_s, argv[1]);
    }

    list_dir(path, flag);

    return 0;
}
