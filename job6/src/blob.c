#include "utils/std.h"
#include "utils/file.h"
#include "oid.h"
#include "cmd.h"
#include "config.h"
#include "blob.h"

bool store_blob(char *source_path, oid_t *oid)
{
    get_file_oid(source_path, oid);
    if (oid_exists(oid))
        return false;

    FILE *source_f = fopen(source_path, "r"); 
    FILE *target_f = fopen_oid(oid, "w");

    // 1. type
    fputs("blob\n", target_f);

    // 2. file_size
    int file_size = get_file_size(source_path);
    fprintf(target_f, "%d\n", file_size);

    // 3. content
    while (1) {
        char buff[1024];
        int count = fread(buff, 1, sizeof(buff), source_f);
        if (count == 0)
            break;
        fwrite(buff, 1, count, target_f);
    }

    fclose(source_f);
    fclose(target_f);
    return true;
}

void copy_blob_content(oid_t *oid, char *target_path)
{
    FILE *source_f = fopen_oid(oid, "r");
    FILE *target_f = fopen(target_path, "w");

    char line[LINE_SIZE];
    read_line(source_f, line);
    read_line(source_f, line);

    int c;
    while ((c = fgetc(source_f)) != EOF)
        fputc(c, target_f);

    fclose(source_f);
    fclose(target_f);
}
