//server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "./sock_dgram"

typedef struct
{
    int date;
    int month;
    int temp;
    char destination[20];
} socket_data_t;

int main()
{
    int addr_size = sizeof(struct sockaddr_un);
    printf("Start of UNIX Datagram Server Process\n");

    char receive_data[50];
    socket_data_t send_data = {25, 12, 30, "Hanoi"};

    int server_sock;
    struct sockaddr_un server_addr, client_addr;

    server_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    unlink(SOCKET_PATH); 

    if (bind(server_sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        perror("Failed to bind socket");
        close(server_sock);
        exit(1);
    }

    printf("Ready to transfer data\n");
    while (1)
    {
        socklen_t client_len = sizeof(client_addr);
        if (recvfrom(server_sock, (void *)receive_data, sizeof(receive_data), 0,
                     (struct sockaddr *)&client_addr, &client_len) > 0)
        {
            printf("Received Data: %s", receive_data);
            if (strcmp(receive_data, "Give me sth bro\n") == 0)
            {
                printf("Sending Data\n");
                sendto(server_sock, (const void *)&send_data, sizeof(send_data), 0,
                       (const struct sockaddr *)&client_addr, client_len);
            }
        }
    }
    close(server_sock);
    unlink(SOCKET_PATH);
    return 0;
}