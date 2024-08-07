#include "std.h"
#include "http.h"
#include "errno.h"

char *web_root = "www";

// http_write_chunk(fw, "hello", 5)
// 共发送 3+5+2 个字符
// 5\r\nHELLO\r\n
void http_write_chunk(FILE *fw, void *chunk, int size)
{
    fprintf(fw, "%x\r\n", size);
    fwrite(chunk, size, 1, fw);
    fprintf(fw, "\r\n");
}

// 共发送 3+0+2 个字符
// 0\r\n\r\n
void http_end(FILE *fw)
{
    http_write_chunk(fw, NULL, 0);
    fflush(fw);
}

void http_prints(FILE *fw, void *string)
{
    int size = strlen(string);
    http_write_chunk(fw, string, size);
}

void http_printf(FILE *fw, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char chunk[100];
    int size = vsprintf(chunk, format, ap);
    va_end(ap);

    http_write_chunk(fw, chunk, size);
}

void http_send_headers(FILE *fp, char *content_type)
{
    fprintf(fp, "HTTP/1.1 200 OK\r\n");
    fprintf(fp, "Server: tiny httpd\r\n");
    fprintf(fp, "Content-type: %s\r\n", content_type);
    fprintf(fp, "Transfer-Encoding: chunked\r\n");
    fprintf(fp, "\r\n");
}

// GET /index.html HTTP/1.1\r\n
char *http_parse_req(FILE *fr, char *req, int req_size)
{
    fgets(req, req_size, fr);
    char *method = strtok(req, " ");
    char *path = strtok(NULL, " ");
    char *protocol = strtok(NULL, "\r");
    if (strcmp(path, "/") == 0) // if path is "/" replace it with "/index.html"
    {
        path = "/index.html";
    }
    static char http_path[1024];
    sprintf(http_path, "%s%s", web_root, path); // prepend "www" to the path
    return http_path;
}

// 处理文件
char *generate_directory_listing(const char *real_path, const char *req_path)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(real_path);
    if (d == NULL)
    {
        return NULL;
    }

    size_t html_size = 4096; // Increase initial buffer size.
    char *html = malloc(html_size);
    if (html == NULL)
    {
        closedir(d);
        return NULL;
    }

    size_t length = sprintf(html, "<html><body><h1>Index of %s</h1><hr>", req_path);

    while ((dir = readdir(d)) != NULL)
    {
        if (strcmp(dir->d_name, ".") == 0)
            continue;

        if (strcmp(dir->d_name, "..") == 0)
            continue;

        size_t new_length = length + strlen(dir->d_name) * 2 + strlen(req_path) + 27; // Calculate new length after adding the directory name and request path.
        if (new_length >= html_size)
        { // If buffer is not large enough, increase its size.
            html_size *= 2;
            char *new_html = realloc(html, html_size);
            if (new_html == NULL)
            { // If realloc failed, clean up and return.
                free(html);
                closedir(d);
                return NULL;
            }
            html = new_html;
        }
        length += sprintf(html + length, "<a href=\"%s/%s\">%s</a><br>", req_path, dir->d_name, dir->d_name); // Append directory name with request path as prefix.
    }

    sprintf(html + length, "<hr></body></html>"); // Append closing tags.
    closedir(d);
    return html;
}

void http_handler(int fd)
{
    FILE *fr = fdopen(fd, "r");
    FILE *fw = fdopen(fd, "w");

    char req[1024];
    char *http_path;
    http_path = http_parse_req(fr, req, sizeof(req));
    fprintf(stderr, "http path: %s\n", http_path);
    char *app_prefix = "www/app";
    if (strncmp(http_path, app_prefix, strlen(app_prefix)) == 0)
    {
        // Get the script name by removing the "/app/" prefix
        char *script_name = http_path;

        if (strstr(script_name, "show_env") != NULL)
        {
            script_name = "www/app/show_env";
        }
        else if (strstr(script_name, "add_student?") != NULL || strstr(script_name, "remove_student?") != NULL)
        {
            char *cmd;
            if (strstr(script_name, "add_student?") != NULL)
            {
                cmd = "./www/app/add_student";
            }
            else
            {
                cmd = "./www/app/remove_student";
            }
            // Get the query by removing the "/add_student?" prefix
            char *query = script_name + strlen(cmd) - 1;
            char envv_query[1024];
            sprintf(envv_query, "QUERY_STRING=%s", query);
            fprintf(stderr, "envv_query: %s\n", envv_query);
            // Create a pipe
            int pipefd[2];
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            // Fork a new process
            int pid = fork();
            if (pid == 0)
            {
                // Child process
                close(pipefd[0]);               // Close unused read end
                dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the pipe

                const char *argv[] = {cmd, NULL};
                const char *envv[] = {envv_query, NULL};
                execve(argv[0], (char *const *)argv, (char *const *)envv);
                perror("execve");
                exit(1); // If execve returns, it must have failed.
            }
            else if (pid < 0)
            {
                // Failed to fork
                fprintf(stderr, "Error forking process: %s\n", strerror(errno));
                return;
            }
            else
            {
                // Parent process
                close(pipefd[1]); // Close unused write end

                // Read from the pipe and write to the HTTP response
                http_send_headers(fw, "text/html");
                char buffer[1024];
                size_t bytes;
                while ((bytes = read(pipefd[0], buffer, sizeof(buffer))) > 0)
                {
                    http_write_chunk(fw, buffer, bytes);
                }

                int status;
                waitpid(pid, &status, 0);

                // Check if child process exited normally
                if (WIFEXITED(status))
                {
                    fprintf(stderr, "Script exit code: %d\n", WEXITSTATUS(status));
                }
                else
                {
                    fprintf(stderr, "Script did not exit normally\n");
                }
                http_end(fw);
                return;
            }
        }

        fprintf(stderr, "script_name: %s\n", script_name);
        // Form the actual script path
        char script_path[1024];
        sprintf(script_path, "./%s", script_name);         // add the './' before the script path
        fprintf(stderr, "Script path: %s\n", script_path); // Log the actual script path

        // Open a pipe to execute the script and get the output
        FILE *pipe = popen(script_path, "r");
        if (pipe == NULL)
        {
            fprintf(stderr, "Error executing script: %s\n", strerror(errno)); // Log any error that occurs
            fprintf(fw, "HTTP/1.1 500 Internal Server Error\r\n\r\n");
            return;
        }
        http_send_headers(fw, "text/html");
        char buffer[1024];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), pipe)) > 0)
        {
            http_write_chunk(fw, buffer, bytes);
        }
        int ret = pclose(pipe);
        fprintf(stderr, "Script exit code: %d\n", ret); // Log the script's exit code
    }
    else
    {
        struct stat path_stat;
        stat(http_path, &path_stat);
        if (S_ISDIR(path_stat.st_mode))
        {                                                                // If the path is a directory
            char *html = generate_directory_listing(http_path, req + 4); // Pass the original request path without "www" prefix.
            if (html == NULL)
            {
                fprintf(fw, "HTTP/1.1 404 Not Found\r\n\r\n");
                return;
            }
            http_send_headers(fw, "text/html");
            http_prints(fw, html);
            free(html);
        }
        else
        { // If the path is a file
            FILE *file = fopen(http_path, "r");
            if (file == NULL)
            {
                fprintf(fw, "HTTP/1.1 404 Not Found\r\n\r\n");
                return;
            }
            http_send_headers(fw, "text/html");
            char buffer[1024];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
            {
                http_write_chunk(fw, buffer, bytes);
            }
            fclose(file);
        }
    }

    http_end(fw);
}