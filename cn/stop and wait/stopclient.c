#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>

#define PORT 8100
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 2
#define WINDOW_SIZE 4

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int base = 1; // Base sequence number of the window
    int next_seqnum = 1; // Next sequence number to be sent

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    while (base <= 5) { // We have 5 frames to send
        
       
            sprintf(buffer, "SEQ%d", base);
            printf("Sending frame: %s\n", buffer);
            send(client_socket, buffer, strlen(buffer), 0);
            

        // Set timeout for receiving acknowledgment
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;
        if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
            perror("Error setting timeout");
            exit(EXIT_FAILURE);
        }

        // Receive acknowledgment from server
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0 && strncmp(buffer, "ACK", 3) == 0) {
            int ack_num;
            sscanf(buffer, "ACK%d", &ack_num);
            printf("Received acknowledgment for frame: SEQ%d\n", ack_num);
            if (ack_num == base) {
                base++; // Move the window
                next_seqnum = base; // Update next sequence number
                if (next_seqnum > 5) {
                    next_seqnum = 5;
                }
            }
        } else {
            printf("Timeout or acknowledgment not received. Retransmitting frames in window...\n");
            // Retransmit frames in window
            next_seqnum = base; // Reset next_seqnum to base for retransmission
        }
    }

    close(client_socket);
    return 0;
}

