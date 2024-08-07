#ifndef _UTILS_FILE_H
#define _UTILS_FILE_H

#define LINE_SIZE 128

extern int read_line(FILE *f, char *line);
extern void write_line(FILE *f, char *line);
extern int load_line(char *path, char *line);
extern void store_line(char *path, char *line);

extern bool file_is_dir(char *path);
extern bool file_exists(char *path);
extern int get_file_size(char *path);
extern void make_file(char *path, char *content);
extern void copy_file(char *source_path, char *target_path);

#endif
