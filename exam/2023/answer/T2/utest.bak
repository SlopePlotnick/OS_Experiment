#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include "utest.h"

typedef struct {
    int id;
    char *desc;
    void (*fp)();
    int ok;
} utest_t;

static utest_t utest_array[100];
static int utest_count;
static int utest_log;

void utest_add(char *desc, void (*fp)())
{
    utest_t *utest = utest_array + utest_count;
    utest->id = utest_count;
    utest->desc = strdup(desc);
    utest->fp = fp;
    utest_count++;
}

void utest_exec(utest_t *utest)
{
    printf("%03d: %-32s", utest->id, utest->desc);
    fflush(stdout);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(utest_log, 1);
        dup2(utest_log, 2);
        close(utest_log);

        utest->fp(); // NO ERROR
        exit(EXIT_SUCCESS); // Reach here
    }

    int status;
    int error = waitpid(pid, &status, 0);
    if ((error > 0) && WIFEXITED(status) && (WEXITSTATUS(status) == 0))
        utest->ok = 1;
    else
        utest->ok = 0;

    char *result;
    if (utest->ok)
        result = "O";
    else
        result = "X";
    printf("%s\n", result);
}

// need not modify
void utest_report(int pass_all_test)
{
    if (pass_all_test) {
        printf("Pass all the test.\n");
        return;
    }

    printf("The following test failed:\n");
    for (int i = 0; i < utest_count; i++) {
        utest_t *utest = utest_array + i;
        if (utest->ok == 0)
            printf("%03d: %-32s\n", utest->id, utest->desc);
    }
    printf("\n");
    printf("See utest.log for details.\n");
}

void utest_dump_log()
{
    printf("Contents of utest.log:\n");
    lseek(utest_log, 0, SEEK_SET);
    while (1) {
        char buff[1024];
        int count = read(utest_log, buff, sizeof(buff));
        if (count <= 0)
            break;
        write(1, buff, count);
    }
}

// need not modify
void utest_run(void)
{
    utest_log = open("utest.log", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0664);
    if (utest_log < 0) {
        perror("open");
        printf("open utest.log failed\n");
        exit(EXIT_FAILURE);
    }

    int pass_all_test = 1;
    for (int i = 0; i < utest_count; i++) {
        utest_t *utest = utest_array + i;
        utest_exec(utest);

        if (utest->ok == 0)
            pass_all_test = 0;
    }
    printf("\n");

    utest_report(pass_all_test);
    close(utest_log);
}
