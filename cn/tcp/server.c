#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main()
{
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int port = 8080;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t addr_size = sizeof(client_addr);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);

    while (1)
    {
    	memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Client: %s\n", buffer);
        printf("Enter server's message: ");
        scanf("%s", buffer);

        send(client_socket, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "bye") == 0)
            break;
    }
    close(client_socket);
    close(server_socket);
    return 0;
}

