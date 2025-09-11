#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define SRC_IP     "10.46.172.56"   // IP máy gửi (máy A)

int main() {
    // 1. Tạo raw socket, protocol phải trùng với sender (253)
    int sockfd = socket(AF_INET, SOCK_RAW, 253);
    if (sockfd < 0) { perror("socket"); exit(1); }

    // 2. Buffer lớn để nhận dữ liệu (header + payload)
    char buffer[4096];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);

    printf("Listening for raw packets (protocol 253)...\n");
    while (1) {
        // 3. Nhận gói tin (blocking)
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                         (struct sockaddr *)&from, &fromlen);
        if (n > 0) {
            // 4. Trích xuất IP header
            struct iphdr *iph = (struct iphdr *)buffer;
            char *data = buffer + iph->ihl*4; // Payload nằm sau header

            // 5. Kiểm tra IP nguồn (chỉ in ra nếu đúng máy gửi)
            if (iph->saddr == inet_addr(SRC_IP)) {
                printf("Received from %s: %s\n", inet_ntoa(from.sin_addr), data);
            }
        }
    }
    close(sockfd);
    return 0;
}
