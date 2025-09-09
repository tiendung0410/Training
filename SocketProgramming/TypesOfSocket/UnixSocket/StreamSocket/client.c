// client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "./sock_stream"

int main() {
    char send_data[100];
    char received_data[100];

    printf("Start of UNIX Socket Client Process\n");

    int client_fd;
    struct sockaddr_un server_addr;

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    printf("Prepare to request connect\n");
    if (connect(client_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
        printf("Connected to server\n");
        while (1) {
            printf("Enter data to send: ");
            fgets(send_data, sizeof(send_data), stdin);
            write(client_fd, send_data, sizeof(send_data));
            if (strncmp(send_data, "exit", 4) == 0) {
                printf("Disconnecting from server\n");
                close(client_fd);
                break;
            }
            if (read(client_fd, received_data, sizeof(received_data)) > 0) {
                printf("Received data: %s\n", received_data);
            }
        }
    } else {
        perror("Connection failed");
        close(client_fd);
    }
    return 0;
}