#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

// 判断当前path所指的是普通文件还是目录
// 0. 普通文件
// 1. 目录
int judge(char *path)
{
    int ans;

    struct stat attr;
    if (stat(path, &attr) == 0)
    {
        if (S_ISDIR(attr.st_mode))
            ans = 1; // 目录
        else
            ans = 0; // 普通文件
    }
    else
    {
        // 处理stat失败的情况
        ans = -1;
    }

    return ans;
}

void search_simple(char *path, char *word)
{
    FILE *f = fopen(path, "r");
    assert(f);

    char line[256];
    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, word))
        {
            // 处理掉换行符
            char *nl = strchr(line, '\n');
            if (nl)
                *nl = 0;

            printf("%s\n", line);
        }
    }

    fclose(f);
}

void search(char *path, char *word, int flag)
{
    if (!flag)
    {
        FILE *f = fopen(path, "r");
        assert(f);

        char line[256];
        while (fgets(line, sizeof(line), f))
        {
            if (strstr(line, word))
            {
                // 处理掉换行符
                char *nl = strchr(line, '\n');
                if (nl)
                    *nl = 0;

                printf("%s: %s\n", path, line);
            }
        }

        fclose(f);
    }
    else
    {
        // 递归查找
        // 当前的path是目录的路径
        // 检索当前目录下的所有项
        DIR *dir = opendir(path);
        struct dirent *entry;
        while (entry = readdir(dir))
        {
            if (strcmp(entry->d_name, ".") == 0)
                continue;

            if (strcmp(entry->d_name, "..") == 0)
                continue;

            // 解算出被检索到的项的路径
            char path_s[100];
            strcpy(path_s, path);
            char *tmp = strcat(path_s, "/");
            char tmp_s[100];
            strcpy(tmp_s, tmp);
            char *new_path = strcat(tmp_s, entry->d_name);

            if (entry->d_type == DT_DIR)
                // 当前项为目录 flag=1 递归调用
                search(new_path, word, 1);

            if (entry->d_type == DT_REG)
                // 当前项为普通文件
                search(new_path, word, 0);
        }
        closedir(dir);
    }
}

int main(int argc, char *argv[])
{
    int flag = 0;         // 是否递归
    char *word; // 待查找子串
    char *path;           // 待搜索路径
    if (argc == 3)
    {
        path = argv[2];
        word = argv[1];
        flag = 0;
    }
    else if (argc == 4 && !(strcmp(argv[1], "-r")))
    {
        // 递归查找
        path = argv[3];
        word = argv[2];
        // 当-r后面的参数为普通文件的路径时 视为没有-r选项处理
        if (!(judge(path)))
            flag = 0;
        else
            flag = 1;
    }

    if (flag == 0)
        search_simple(path, word);
    else
        search(path, word, flag);

    return 0;
}