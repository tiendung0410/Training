#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080
#define MAX_PENDING_CONNECTIONS 5
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    const char *message = "Hello from server";

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address parameters
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Set the server socket to non-blocking mode
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_PENDING_CONNECTIONS) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept new connections
        while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) >= 0) {
            printf("New connection, socket fd is %d\n", new_socket);

            // Send message to client ( comment this to test non-blocking on client side)
            int bytes_sent = send(new_socket, message, strlen(message), 0);
            if (bytes_sent < 0) {
                perror("send failed");
            }

            close(new_socket); // Close the connection
        }

        // Handle non-blocking accept errors
        if (new_socket < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("accept failed");
            break;
        }

        // Simulate some non-blocking work (e.g., checking timers, other tasks)
        usleep(1000); // Sleep briefly to avoid busy-waiting
    }

    close(server_fd);
    return 0;
}