#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main()
{
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket==-1)
    {
    	perror("Client_socket");
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    int connection_status = connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Connection status: %d\n", connection_status);

    int i = 1;
    while (i <= 10)
    {

        printf("Enter client's message: ");
        scanf("%s", buffer);
        send(client_socket, buffer, strlen(buffer), 0);

        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "bye") == 0)
            break;

        i++;
    }
    close(client_socket);
    return 0;
}

