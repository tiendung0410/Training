#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

void main(int argc, char** argv)
{
    char send_data[100];
    char received_data[100];
    printf("Start of Client Process\n");

    int client_fd;
    struct sockaddr_in  server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    client_fd = socket(AF_INET,SOCK_STREAM,0);
    printf("Prepare to request connect\n");
    if (connect(client_fd,(const struct sockaddr *)&server_addr,sizeof(server_addr)) ==0)
    {  
        printf("Connected to server\n");
        while (1)
        {
            printf("Enter data to send: ");
            fgets(send_data, sizeof(send_data), stdin);
            write(client_fd,send_data,sizeof(send_data));
            if(strncmp(send_data,"exit",4) == 0)
            {
                printf("Disconnecting from server\n");
                close(client_fd);
                break;
            }
            if(read(client_fd,received_data,sizeof(received_data)) >0)
            {
                printf("Received data: %s\n",received_data);
            }
        }
    }
    else
    {
        printf("Connection failed\n");
        close(client_fd);
    }
    exit(0);

}