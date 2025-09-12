#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define DEST_IP "127.0.0.1"   // IP máy nhận (B)
#define SRC_IP  "127.0.0.1"   // IP máy gửi (A)

// Hàm tính checksum cho IP header (chuẩn RFC 791)
unsigned short csum(unsigned short *buf, int nwords) {
    unsigned long sum = 0;
    for (; nwords > 0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);  // Cộng dồn bit cao vào bit thấp
    sum += (sum >> 16);                  // Cộng dồn lần nữa nếu vẫn còn bit cao
    return (unsigned short)(~sum);       // Đảo bit cuối
}

int main() {
    // 1. Tạo raw socket với protocol=IPPROTO_RAW
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd < 0) { perror("socket"); exit(1); }

    // 2. Báo cho kernel biết sẽ tự tạo IP header (IP_HDRINCL)
    int one = 1;
    setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));

    // 3. Tạo buffer chứa toàn bộ packet (header + payload)
    char packet[4096];
    memset(packet, 0, sizeof(packet)); // Đảm bảo sạch bộ nhớ

    // 4. Gán struct iphdr vào đầu buffer
    struct iphdr *iph = (struct iphdr *)packet;

    // 5. Gán payload nằm sau header IP
    char *data = packet + sizeof(struct iphdr);
    int datalen = sprintf(data, "HELLO FROM RAW SOCKET!\n");

    // 6. Điền thông tin IP header
    iph->ihl = 5;                   // Độ dài header (5*4=20 bytes)
    iph->version = 4;               // IPv4
    iph->tos = 0;                   // Không ưu tiên đặc biệt
    iph->tot_len = htons(sizeof(struct iphdr) + datalen); // Tổng chiều dài packet
    iph->id = htons(100);           // Packet ID (tùy ý)
    iph->frag_off = 0;              // Không phân mảnh
    iph->ttl = 64;                  // Thời gian sống (64 hop)
    iph->protocol = 253;            // Protocol tùy chọn (253 = experimental, tránh bị OS xử lý)
    iph->saddr = inet_addr(SRC_IP); // IP nguồn
    iph->daddr = inet_addr(DEST_IP);// IP đích

    // 7. Tính checksum IP header (bắt buộc chuẩn IP)
    iph->check = csum((unsigned short *)iph, sizeof(struct iphdr)/2);

    // 8. Thiết lập sockaddr_in cho địa chỉ đích
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = iph->daddr;

    // 9. Gửi packet qua mạng tới máy B
    if (sendto(sockfd, packet, sizeof(struct iphdr) + datalen, 0,
               (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        perror("sendto");
    } else {
        printf("Sent raw packet to %s\n", DEST_IP);
    }

    // 10. Đóng socket
    close(sockfd);
    return 0;
}
