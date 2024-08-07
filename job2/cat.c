#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    int fd;                       // 文件标识符
    fd = open(argv[1], O_RDONLY); // 打开文件
    assert(fd >= 0);              // fd小于0说明打开文件失败 报错终止程序
    char ch;                      // 用于读取字符的缓冲区
    while (read(fd, &ch, sizeof(ch)))
    {
        // read函数返回值不为0表示未读取到文件末尾 且会随着读取自动往后移动读取的位置
        printf("%c", ch); // 打印字符
    }
    printf("\n"); // 读取完后输出空行 是否要加这行有待考虑

    close(fd); // 关闭文件

    return 0;
}