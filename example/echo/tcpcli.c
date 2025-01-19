// tcp client using thread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_MSG_SIZE 1024

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in server_addr;
    char message[MAX_MSG_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
        return 1;
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        return 1;
    }

    printf("Connected to server\n");
    printf("Enter message: ");

    while (1) {
        fgets(message, MAX_MSG_SIZE, stdin);

        if (send(client_socket, message, strlen(message), 0) == -1) {
            perror("send failed");
            return 1;
        }

        if (recv(client_socket, message, MAX_MSG_SIZE, 0) == -1) {
            perror("recv failed");
            return 1;
        }

        printf("Server reply: %s", message);
    }

    close(client_socket);
    return 0;
}