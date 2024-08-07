#include <string.h>

/* input string will be destroyed */
int split_string(char *string, char *delim, char *word_table[])
{
    char *word;
    int word_count = 0;

    word = strtok(string, delim);
    while (word) {
        word_table[word_count++] = word;
        word = strtok(NULL, delim);
    }
    return word_count;
}
