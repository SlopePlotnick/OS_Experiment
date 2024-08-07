#include "std.h"
#include "tcp.h"
#include "http.h"
#include "signal.h"
#include "sys/wait.h"

int main(int argc, char *argv[])
{
    char *ip_address = "127.0.0.1";
    int port = 8080;
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            i++;
            port = atoi(argv[i]);
            continue;
        }
    }

    // 创建一个 TCP 服务器来监听指定的 IP 地址和端口
    int server_fd = tcp_listen(ip_address, port);

    while (1) {
        // 接受一个新的连接
        int client_fd = tcp_accept(server_fd);

        // 使用fork创建子进程处理客户端连接
        int pid = fork();

        if (pid < 0) {
            // Fork失败
            perror("fork");
            close(client_fd);
        } else if (pid == 0) {
            // 子进程中
            // 处理 HTTP 请求
            http_handler(client_fd);
            // 处理完后关闭连接
            close(client_fd);
            // 子进程任务完成，退出
            exit(0);
        } else {
            // 父进程中
            // 关闭客户端套接字，父进程不需要这个套接字
            close(client_fd);
        }
    }

    return 0;
}
