#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main() {
    struct sockaddr_in serveraddr;
    int clientsock, length;
    char filename[100], filedata[300];
    int output_fd;

    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(4040);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    clientsock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsock < 0) {
        perror("Error creating socket");
        exit(1);
    }

    if (connect(clientsock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Error connecting to server");
        close(clientsock);
        exit(1);
    }

    printf("Enter the name of the file to request from the server: ");
    scanf("%s", filename);
    length = strlen(filename);

    if (write(clientsock, filename, length) < 0) {
        perror("Error writing to socket");
        close(clientsock);
        exit(1);
    }

    int rdret = read(clientsock, filedata, sizeof(filedata) - 1);
    if (rdret < 0) {
        perror("Error reading from socket");
        close(clientsock);
        exit(1);
    }

    filedata[rdret] = '\0'; // Null-terminate the received data

    // Print the contents of the file (optional)
    printf("\nThe contents of the file:\n\n");
    printf("%s", filedata);

    // Save the received data to a file in the client's directory
    output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (output_fd < 0) {
        perror("Error opening output file");
        close(clientsock);
        exit(1);
    }

    if (write(output_fd, filedata, rdret) < 0) {
        perror("Error writing to output file");
        close(output_fd);
        close(clientsock);
        exit(1);
    }

    close(output_fd);
    close(clientsock);
    return 0;
}

