
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
#include <sys/select.h>
#include <fcntl.h>


#define PORT 8080
#define RQ_LIMIT 5
#define THREAD_POOL_SIZE 2
#define FDS_SIZE 10

pthread_t thread_pool[THREAD_POOL_SIZE];
int thread_client_socket[THREAD_POOL_SIZE][FDS_SIZE]={0};
int thread_client_index[THREAD_POOL_SIZE]={0};



int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void* communication_handler(void *argument)
{
    int id;
    for(int i=0;i<THREAD_POOL_SIZE;i++)
    {
        if(pthread_self() == thread_pool[i])
        {
            id = i;
            break;
        }
    }
    fd_set readfds;    
    int activity;
    while (1)
    {
        FD_ZERO(&readfds);
        int max_fd=0;
        while(thread_client_index[id]==0);
        for(int i=0;i<thread_client_index[id];i++)
        {
            if(thread_client_socket[id][i] > max_fd)
            {
                max_fd = thread_client_socket[id][i];
            }
            FD_SET(thread_client_socket[id][i],&readfds);
        }
        activity = select(max_fd+1, &readfds, NULL, NULL, NULL);
        if(activity <0)
        {
            printf("Select error in thread %d\n",id);
            continue;
        }
        for(int i=0;i<thread_client_index[id];i++)
        {
            if(thread_client_socket[id][i] >0 && FD_ISSET(thread_client_socket[id][i],&readfds))
            {
                char buffer[100];
                memset(buffer,0,sizeof(buffer));
                int valread = read(thread_client_socket[id][i],buffer,sizeof(buffer));
                if(valread >0)
                {
                    if(strncmp(buffer,"exit",4)==0)
                    {
                        thread_client_socket[id][i]=0;
                        pthread_exit(0);
                        //expect to rearrange the array but too lazy for that
                    }
                    char send_data[128];
                    sprintf(send_data,"Client %d: %s",thread_client_socket[id][i],buffer);
                    for(int k=0;k<THREAD_POOL_SIZE;k++)
                    {
                        for(int j=0;j<thread_client_index[k];j++)
                        {
                            if(thread_client_socket[k][j]>0 && thread_client_socket[k][j] != thread_client_socket[id][i])
                            {
                                write(thread_client_socket[k][j], send_data, strlen(send_data));
                            }
                        }
                    }
                }
            }
        }
    }

    pthread_detach(pthread_self());
}

void main(int argc,char** argv)
{
    printf("Start of Process!\n");
    int sockfd,client_fd,max_fd;
    struct sockaddr_in  server_addr;
    fd_set server_readfd;


    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd <0)
    {
        printf("Failed to create socket\n");
    }
    set_nonblocking(sockfd);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(sockfd,(const struct sockaddr *)&server_addr,sizeof(server_addr))!=0)
    {
        printf("Failed to binding socket\n");
    }
    
    listen(sockfd,RQ_LIMIT);
    FD_ZERO(&server_readfd);
    FD_SET(sockfd,&server_readfd);
    max_fd = sockfd;
    printf("Listening...\n");

    for(int i=0;i<THREAD_POOL_SIZE;i++)
    {
        pthread_create(&thread_pool[i],NULL,communication_handler,NULL);
    }
    while(1)
    {
        select(max_fd+1,&server_readfd,NULL,NULL,NULL);
        if(FD_ISSET(sockfd,&server_readfd))
        {
            client_fd = accept(sockfd,(struct sockaddr *)NULL,(socklen_t*)NULL);
            if(client_fd >0)
            {
                set_nonblocking(client_fd);
                int id = client_fd % THREAD_POOL_SIZE;
                thread_client_socket[id][thread_client_index[id]] = client_fd;
                thread_client_index[id]++;
                printf("Client fd accepted: %d\n",client_fd);
            }
        }
        sleep(1);
    }

}