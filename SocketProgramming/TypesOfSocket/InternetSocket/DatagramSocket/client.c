//Client src
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>

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
    printf("Start of Client Process\n");

    char request_buf[50]="Give me sth bro\n";
    socket_data_t receive_data;

    int client_sock;
    struct sockaddr_in server_addr,client_addr;

    client_sock=socket(AF_INET,SOCK_DGRAM,0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    server_addr.sin_addr.s_addr = inet_addr("10.46.172.56");

    printf("Ready to transfer data\n");
    while(1)
    {
        printf("Enter to request data\n");
        getchar();
        sendto(client_sock, (const void *)request_buf, sizeof(request_buf), 0, (const struct sockaddr *)&server_addr,sizeof(server_addr));

        if (recvfrom(client_sock, (void *)&receive_data, sizeof(receive_data),0, NULL,NULL) >0)
        {
            printf("Received Data\n");
            printf("Date: %d/%d\n",receive_data.date,receive_data.month);
            printf("Temp: %d\n",receive_data.temp);
            printf("Destination: %s\n",receive_data.destination);
        }
    }
}