#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char *message = "Hello from client";

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address parameters
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDRESS, &serv_addr.sin_addr) <= 0) {
        perror("invalid address / address not supported");
        exit(EXIT_FAILURE);
    }

    // Set the socket to non-blocking mode
    if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl failed");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        // Non-blocking connect will return immediately
        // Check errno to distinguish between connection in progress and connection failed
        if (errno != EINPROGRESS) {
            perror("connection failed");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for connection to complete
    sleep(1);

    // Send message to server
    send(sock, message, strlen(message), 0);
    printf("Message sent to server.\n");

    // Read response from server
    read(sock, buffer, BUFFER_SIZE);
    printf("Server response: %s\n", buffer);

    // Close socket
    close(sock);

    return 0;
}
