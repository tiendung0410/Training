#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>    
#include <sys/select.h>
#include <fcntl.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int client_fd;

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void* receive_message_handler(void *argument)
{
    while(1)
    {
        fd_set readfd;
        FD_ZERO(&readfd);
        FD_SET(client_fd, &readfd);
        int max_fd = client_fd;
        int activity = select(max_fd+1, &readfd, NULL, NULL, NULL);
        if(activity > 0 && FD_ISSET(client_fd, &readfd))
        {
            char recv_buff[128];
            memset(recv_buff, 0, sizeof(recv_buff));
            int res = read(client_fd, recv_buff, sizeof(recv_buff));
            if(res > 0)
            {
                printf("                    %s\n", recv_buff);
            }
            else if(res == 0)
            {
                printf("Server closed connection\n");
                close(client_fd);
                break;
            }
            else
            {
                printf("Error in reading socket\n");
            }
        }
    }
    return NULL;
}

void main(int argc, char** argv)
{
    char send_data[100];
    printf("Start of Client Process\n");

    struct sockaddr_in  server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    client_fd = socket(AF_INET,SOCK_STREAM,0);
    //set_nonblocking(client_fd);
    printf("Prepare to request connect\n");
    if (connect(client_fd,(const struct sockaddr *)&server_addr,sizeof(server_addr)) ==0)
    {  
        printf("Connected to server\n");
        printf("\nText sth\n\n");
        pthread_t thread;
        pthread_create(&thread,NULL,receive_message_handler,NULL);
        while (1)
        {
            memset(send_data,0,sizeof(send_data));
            fgets(send_data, sizeof(send_data), stdin);
            write(client_fd,send_data,sizeof(send_data));
            if(strncmp(send_data,"exit",4) == 0)
            {
                printf("Disconnecting from server\n");
                close(client_fd);
                break;
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