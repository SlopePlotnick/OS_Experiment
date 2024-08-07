#include "std.h"
#include "file.h"

int read_line(FILE *f, char *line)
{
    if (fgets(line, LINE_SIZE, f) == NULL)
        return 0;

    char *nl = strchr(line, '\n');
    assert(nl != NULL);
    *nl = 0;
    return 1;
}

void write_line(FILE *f, char *line)
{
    fputs(line, f);
    fputc('\n', f);
}

int load_line(char *path, char *line)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
        return 0;
    
    int count = read_line(f, line);
    fclose(f);
    return count;
}

void store_line(char *path, char *line)
{
    FILE *f = fopen(path, "w");
    write_line(f, line);
    fclose(f);
}

bool file_is_dir(char *path)
{
    struct stat info;
    stat(path, &info);

    return S_ISDIR(info.st_mode);
}

bool file_exists(char *path)
{
    return access(path, F_OK) == 0;
}

int get_file_size(char *path)
{
    struct stat info;
    stat(path, &info);
    return info.st_size;
}

void make_file(char *path, char *content)
{
    FILE *f = fopen(path, "w");
    // fputs dont add an extra '\n'
    fputs(content, f); 
    fclose(f);
}

void copy_file(char *source_path, char *target_path)
{
    FILE *source_f = fopen(source_path, "r");
    FILE *target_f = fopen(target_path, "w");

    char buff[1024];
    int count;
    while (count = fread(buff, 1, sizeof(buff), source_f))
        fwrite(buff, 1, count, target_f);

    fclose(source_f);
    fclose(target_f);
}
