/*
1. socket() <sys/socket.h>
2. connect() <sys/socket.h>
3. read() and write() <unistd.h>
4. close()  <unistd.h>
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5500

struct data_employee {
    char name[100];
    int age;
    char address[100];
    char position[100];
    int salary;
};

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    struct data_employee employee;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    } else {
        printf("Client started\n");
    }

    // create TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    } else {
        printf("Socket created successfully\n");
    }

    // create server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        exit(1);
    } else {
        printf("Server address created successfully\n");
    }

    // connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(1);
    } else {
        printf("Connected to server\n");
    }

    // send data to server
    while (1) {
        system("clear");
        printf("Enter employee data...\n");

        printf("Name: ");
        fgets(employee.name, sizeof(employee.name), stdin);
        employee.name[strcspn(employee.name, "\n")] = 0;

        printf("Age: ");
        scanf("%d", &employee.age);
        getchar();

        printf("Address: ");
        fgets(employee.address, sizeof(employee.address), stdin);
        employee.address[strcspn(employee.address, "\n")] = 0;

        printf("Position: ");
        fgets(employee.position, sizeof(employee.position), stdin);
        employee.position[strcspn(employee.position, "\n")] = 0;

        printf("Salary: ");
        scanf("%d", &employee.salary);
        getchar();

        if (write(sockfd, &employee, sizeof(employee)) < 0) {
            perror("Write error");
            close(sockfd);
            exit(1);
        } else {
            printf("Data sent to server\n");
            printf("-------------------\n");
            printf("Name: %s\n", employee.name);
            printf("Age: %d\n", employee.age);
            printf("Address: %s\n", employee.address);
            printf("Position: %s\n", employee.position);
            printf("Salary: %d\n", employee.salary);
            printf("-------------------\n");
        }
        
        printf("continue? (y/n): ");
        char c;
        scanf("%c", &c);
        getchar();
        if (c == 'n') {
            break;
        }
    }

    // close connection
    close(sockfd);

    return 0;
}
