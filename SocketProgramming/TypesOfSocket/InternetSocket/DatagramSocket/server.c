//Server src
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct
{
    int date;
    int month;
    int temp;
    char destination[20];
}socket_data_t;

void main(int argc, char**argv)
{
    int port_num = atoi(argv[1]);
    int addr_size = sizeof(struct sockaddr_in);
    printf("Start of Server Process\n");

    char receive_data[50];
    socket_data_t send_data={25,12,30,"Hanoi"};

    int server_sock;
    struct sockaddr_in server_addr,client_addr;

    server_sock=socket(AF_INET,SOCK_DGRAM,0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock,(const struct sockaddr *)&server_addr,sizeof(server_addr));
    printf("Ready to transfer data\n");
    while(1)
    {
        if (recvfrom(server_sock, (void *)receive_data, sizeof(receive_data),0, (struct sockaddr *)&client_addr,(socklen_t *)&addr_size ) >0)
        {
            printf("Received Data: %s",receive_data);
            if(strcmp(receive_data,"Give me sth bro\n")==0)
            {
                printf("Sending Data\n");
                sendto(server_sock, (const void *)&send_data, sizeof(send_data), 0, (const struct sockaddr *)&client_addr,sizeof(client_addr));
            }
        }
    }
}