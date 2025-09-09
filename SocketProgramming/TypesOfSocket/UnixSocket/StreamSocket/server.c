//server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>

#define SOCKET_PATH "./sock_stream"
#define RQ_LIMIT 5

static int client_count = 0;

typedef struct {
    int client_fd;
    int client_id;
} thread_arg_t;

void to_uppercase(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

void *connection_handler(void *argument) {
    thread_arg_t *arg = (thread_arg_t *)argument;
    int sock = arg->client_fd;
    int client_id = arg->client_id;
    char data[100];
    while (1) {
        ssize_t n = read(sock, data, sizeof(data));
        if (n > 0) {
            if (strncmp(data, "exit", 4) == 0) {
                printf("Client %d disconnected\n", client_id);
                close(sock);
                free(arg);
                break;
            }
            printf("Client %d: %s\n", client_id, data);
            to_uppercase(data);
            write(sock, data, sizeof(data));
            memset(data, 0, sizeof(data));
        } else {
            // Client closed connection or error
            close(sock);
            free(arg);
            break;
        }
    }
    pthread_detach(pthread_self());
    return NULL;
}

int main() {
    printf("Start of UNIX Socket Server Process\n");

    int sockfd;
    struct sockaddr_un server_addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    unlink(SOCKET_PATH); // Xóa socket file cũ nếu có

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        perror("Failed to bind socket");
        close(sockfd);
        exit(1);
    }

    listen(sockfd, RQ_LIMIT);
    printf("Listening on %s ...\n", SOCKET_PATH);

    while (1) {
        int com_fd = accept(sockfd, NULL, NULL);
        if (com_fd > 0) {
            client_count++;
            printf("Client %d accepted\n", client_count);
            pthread_t tid;
            thread_arg_t *arg = malloc(sizeof(*arg));
            arg->client_fd = com_fd;
            arg->client_id = client_count;
            pthread_create(&tid, NULL, connection_handler, (void *)arg);
        }
        sleep(1);
    }

    close(sockfd);
    unlink(SOCKET_PATH);
    return 0;
}