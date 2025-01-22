/*
1. socket()
2. bind()
3. listen()
4. accept()
5. read() and write()
6. close()
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 5500
#define BACKLOG 5

#define LOG_ERROR(msg) fprintf(stderr, "[%s:%d] %s: errno=%d (%s)\n", __FILE__, __LINE__, msg, errno, strerror(errno))

struct data_employee {
    char name[100];
    int age;
    char address[100];
    char position[100];
    int salary;
};

int main() {
    int listenfd, connectfd;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    struct data_employee employee;

    printf("Server started\n");

    // create TCP socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        LOG_ERROR("Socket creation failed");
        exit(1);
    } else {
        printf("Successfully: listenfd %d\n", listenfd);
    }

    // create server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "192.168.1.35", &servaddr.sin_addr) <= 0) {
        LOG_ERROR("Error creating server address");
        close(listenfd);
        exit(1);
    } else {
        printf("Server address created successfully\n");
    }

    int opt = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        LOG_ERROR("Setsockopt failed");
        close(listenfd);
        exit(1);
    }

    // bind server address to socket
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        LOG_ERROR("Bind failed");
        close(listenfd);
        exit(1);
    } else {
        printf("Bind successful: ");
        printf("IP: %s | PORT: %d\n", inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
    }

    // listen for incoming connections
    if (listen(listenfd, BACKLOG) == -1) {
        LOG_ERROR("Listen failed");
        close(listenfd);
        exit(1);
    } else {
        printf("Listening on PORT %d ...\n", PORT);
    }

    connectfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
    if (connectfd == -1) {
        LOG_ERROR("Accept failed");
        close(listenfd);
        exit(1);
    } else {
        printf("Client connected: %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    }

    while (1) {
        ssize_t bytes_read = read(connectfd, &employee, sizeof(employee));
        
        if (bytes_read == -1) {
            LOG_ERROR("Read failed");
            break; 
        } else if (bytes_read == 0) {
            printf("Client disconnected.\n");
            break; 
        }

        printf("Employee data received:\n");
        printf("Name: %s\n", employee.name);
        printf("Age: %d\n", employee.age);
        printf("Address: %s\n", employee.address);
        printf("Position: %s\n", employee.position);
        printf("Salary: %d\n", employee.salary);
    }

    // close connection
    close(connectfd);
    // close socket
    close(listenfd);
    
    return 0;
}
