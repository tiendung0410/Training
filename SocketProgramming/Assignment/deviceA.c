#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_ROUTES 8
#define router_name "A"
#define my_port 5000
#define peer_port 5001

typedef struct {
    char destination[16];
    char netmask[16];
    char gateway[16];
    char interface[8];
    int  metric;
} RouteEntry;

void print_table(RouteEntry *table, int n, const char *name) {
    printf("\n=== %s Route Table ===\n", name);
    printf("| %-15s | %-15s | %-15s | %-7s | %-6s |\n",
           "Destination", "Netmask", "Gateway", "Iface", "Metric");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        printf("| %-15s | %-15s | %-15s | %-7s | %-6d |\n",
               table[i].destination, table[i].netmask, table[i].gateway,
               table[i].interface, table[i].metric);
    }
    printf("\n");
}

// Kiểm tra có entry chưa
int find_route(RouteEntry *table, int n, const char *dst, const char *netmask) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(table[i].destination, dst) == 0 && strcmp(table[i].netmask, netmask) == 0)
            return i;
    }
    return -1;
}

int main() {
    RouteEntry table[MAX_ROUTES];
    int route_count = 0;
    char peer_ip[32] = "127.0.0.1";


    // Initialize route table
    strcpy(table[0].destination, "174.16.0.0");
    strcpy(table[0].netmask,    "255.255.0.0");
    strcpy(table[0].gateway,    "0.0.0.0");
    strcpy(table[0].interface,  "Eth0");
    table[0].metric = 1;

    strcpy(table[1].destination, "127.0.0.0");
    strcpy(table[1].netmask,    "255.255.255.0");
    strcpy(table[1].gateway,    "0.0.0.0");
    strcpy(table[1].interface,  "Eth1");
    table[1].metric = 1;

    route_count = 2;

    print_table(table, route_count, router_name);

    // Datagram socket setup
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in my_addr = {0}, peer_addr = {0};
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(my_port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));

    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(peer_port);
    inet_pton(AF_INET, peer_ip, &peer_addr.sin_addr);

    while (1) {
        // 1. Send route table to peer
        sendto(sockfd, table, sizeof(RouteEntry)*route_count, 0,
               (struct sockaddr*)&peer_addr, sizeof(peer_addr));

        printf(">> Sent route table (%d entries) to neighbor!\n", route_count);

        // 2. Wait for route table from peer
        struct sockaddr_in src_addr;
        socklen_t addrlen = sizeof(src_addr);
        RouteEntry recv_table[MAX_ROUTES];
        int num = recvfrom(sockfd, recv_table, sizeof(recv_table), 0,
                           (struct sockaddr*)&src_addr, &addrlen);

        int recv_count = num / sizeof(RouteEntry);
        printf("<< Received route table (%d entries) from neighbor!\n", recv_count);

        // 3. Update new routes into the table (add metric=1, next hop/gateway is peer)
        int updated = 0;
        char neighbor_ip[16];
        inet_ntop(AF_INET, &src_addr.sin_addr, neighbor_ip, sizeof(neighbor_ip));
        for (int i = 0; i < recv_count; ++i) {
            int idx = find_route(table, route_count, recv_table[i].destination, recv_table[i].netmask);
            int new_metric = recv_table[i].metric + 1;
            if (idx == -1) {
                // Add new entry
                strcpy(table[route_count].destination, recv_table[i].destination);
                strcpy(table[route_count].netmask,    recv_table[i].netmask);
                strcpy(table[route_count].gateway,    neighbor_ip);
                for(int j = 0; j < route_count; j++) {  
                    printf("Found interface %s for gateway %s\n", table[j].interface, neighbor_ip);
                    if(strncmp(table[j].destination,neighbor_ip,strlen(neighbor_ip)-2) == 0 ) {
                        strcpy(table[route_count].interface, table[j].interface);
                        break;
                    }
                }
                table[route_count].metric = new_metric;
                route_count++;
                updated = 1;
            } else {
                // if existed, check if better metric
                if (table[idx].metric > new_metric) {
                    strcpy(table[idx].gateway, neighbor_ip);
                    table[idx].metric = new_metric;
                    updated = 1;
                }
            }
        }
        if (updated)
            print_table(table, route_count, router_name);

        sleep(3); 
    }

    close(sockfd);
    return 0;
}
