// client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "./sock_dgram"
#define CLIENT_SOCKET_PATH "./sock_dgram_client"

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
    printf("Start of UNIX Datagram Client Process\n");

    char request_buf[50] = "Give me sth bro\n";
    socket_data_t receive_data;

    int client_sock;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;

    client_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_sock < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    unlink(CLIENT_SOCKET_PATH); // Xóa file socket cũ nếu có

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_SOCKET_PATH, sizeof(client_addr.sun_path) - 1);

    if (bind(client_sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) != 0) {
        perror("Failed to bind client socket");
        close(client_sock);
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    printf("Ready to transfer data\n");
    while (1)
    {
        printf("\nEnter to request data\n");
        getchar();
        sendto(client_sock, (const void *)request_buf, sizeof(request_buf), 0,
               (const struct sockaddr *)&server_addr, sizeof(server_addr));

        if (recvfrom(client_sock, (void *)&receive_data, sizeof(receive_data), 0, NULL, NULL) > 0)
        {
            printf("Received Data\n");
            printf("Date: %d/%d\n", receive_data.date, receive_data.month);
            printf("Temp: %d\n", receive_data.temp);
            printf("Destination: %s\n", receive_data.destination);
        }
    }
    close(client_sock);
    unlink(CLIENT_SOCKET_PATH);
    return 0;
}