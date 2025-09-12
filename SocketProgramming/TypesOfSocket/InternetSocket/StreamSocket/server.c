#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>

#define RQ_LIMIT 5
#define PORT 8080
static int client_count = 0;
typedef struct 
{
    int client_fd;
    int client_id;
}thread_arg_t;


void to_uppercase(char *str)
{
    while(*str)
    {
        *str = toupper(*str);
        str++;
    }
}

void *connection_handler(void *argument)
{
    thread_arg_t *arg = (thread_arg_t *)argument;
    int sock = arg->client_fd;
    int client_id = arg->client_id;
    char data[100];
    while (1)
    {
        if(read(sock,data,sizeof(data)) >0)
        {
            if(strncmp(data,"exit",4) == 0)
            {
                printf("Client %d disconnected\n",client_id);
                close(sock);
                free(arg);
                break;
            }
            printf("Client %d: %s\n",client_id,data);
            to_uppercase(data);
            write(sock,data,sizeof(data));
            memset(data,0,sizeof(data));
        }   
    }
    pthread_detach(pthread_self());
}

void main(int argc,char** argv)
{
    printf("Start of Process!\n");

    int sockfd,data_transfer_fd;
    struct sockaddr_in  server_addr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd <0)
    {
        printf("Failed to create socket\n");
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(sockfd,(const struct sockaddr *)&server_addr,sizeof(server_addr))!=0)
    {
        printf("Failed to binding socket\n");
    }
    
    listen(sockfd,RQ_LIMIT);
    printf("Listening...\n");
    while(1)
    {
        int com_fd = accept(sockfd,(struct sockaddr *)NULL,(socklen_t*)NULL);
        if(com_fd >0)
        {
            client_count++;
            printf("Client %d accepted\n", client_count);
            pthread_t tid;
            thread_arg_t *arg = malloc(sizeof(*arg));
            arg->client_fd = com_fd;
            arg->client_id = client_count;
            pthread_create(&tid,NULL,connection_handler,(void*)arg);
        }
        sleep(1);
    }

}