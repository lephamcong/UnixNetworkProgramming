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

#define PORT 5500

struct data_employee {
    char name[100];
    int age;
    char address[100];
    char position[100];
    int salary;
};

int main() {
    int listenfd, connectfd;
    struct sockaddr_in servaddr;
    struct data_employee employee;

    // create TCP socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    } else {
        printf("Socket created successfully\n");
    }

    // create server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // bind server address to socket
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(1);
    } else {
        printf("Bind successful\n");
    }

    // listen for incoming connections
    if (listen(listenfd, 5) < 0) {
        perror("Listen failed");
        exit(1);
    } else {
        printf("Listen successful\n");
    }

    printf("Server is running...\n");

    // accept and handle connections
    for (;;) {
        connectfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
        if (connectfd < 0) {
            perror("Accept failed");
            continue;
        } else {
            printf("Connection accepted\n");
            system("clear");
        }

        // read data from client
        while (read(connectfd, &employee, sizeof(employee)) > 0) {
            printf("Employee data received\n");
            printf("Name: %s\n", employee.name);
            printf("Age: %d\n", employee.age);
            printf("Address: %s\n", employee.address);
            printf("Position: %s\n", employee.position);
            printf("Salary: %d\n", employee.salary);
            printf("-----------------------\n");
        }

        // close connection
        close(connectfd);
    }

    // close socket
    close(listenfd);
    
    return 0;
}
