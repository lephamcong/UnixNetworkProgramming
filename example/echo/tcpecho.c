// tcp echo server using thread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define MAX_MSG_SIZE 1024

void *client_handler(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[MAX_MSG_SIZE];
    int read_size;

    while ((read_size = recv(client_socket, buffer, MAX_MSG_SIZE, 0)) > 0) {
        printf("Thread ID %ld: %s", pthread_self(), buffer);
        write(client_socket, buffer, read_size);
    }

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }

    free(arg);
    close(client_socket);
    return 0;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    pthread_t client_thread;
    int *client_socket_ptr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    listen(server_socket, MAX_CLIENTS);

    puts("Waiting for incoming connections...");
    while (1) {
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("accept failed");
            return 1;
        }

        puts("Connection accepted");

        client_socket_ptr = malloc(1);
        *client_socket_ptr = client_socket;

        if (pthread_create(&client_thread, NULL, client_handler, (void *)client_socket_ptr) < 0) {
            perror("could not create thread");
            return 1;
        }

        pthread_detach(client_thread);

    }

    close(server_socket);
    return 0;
}