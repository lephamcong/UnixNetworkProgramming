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
#include <errno.h>

#define PORT 5500

#define LOG_ERROR(msg) fprintf(stderr, "[%s:%d] %s: errno=%d (%s)\n", __FILE__, __LINE__, msg, errno, strerror(errno))

struct data_employee {
    char name[100];
    int age;
    char address[100];
    char position[100];
    int salary;
};

int main(int argc, char **argv) {
    int clientfd;
    struct sockaddr_in servaddr;
    struct data_employee employee;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    } else {
        printf("Client started\n");
    }

    // create TCP socket
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        LOG_ERROR("Socket creation failed");
        exit(1);
    } else {
        printf("Successful: clientfd %d\n", clientfd);
    }

    // create server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        LOG_ERROR("Error creating server address");
        close(clientfd);
        exit(1);
    } else {
        printf("Server address created successfully\n");
    }

    // connect to server
    if (connect(clientfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        LOG_ERROR("Connection failed");
        close(clientfd);
        exit(1);
    } else {
        printf("Connected to server: ");
        printf("%s:%d\n", inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
    }

    // send data to server
    while (1) {
        
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

        size_t bytes_sent = write(clientfd, &employee, sizeof(employee));
        if (bytes_sent == -1) {
            LOG_ERROR("Write failed");
            close(clientfd);
            exit(1);
        } else {
            printf("Sent %ld bytes\n", bytes_sent);
        }
        
        printf("Continue? (y/n): ");
        char c;
        scanf("%c", &c);
        getchar();
        if (c == 'n') {
            close(clientfd);
            break;
        }
    }
    
    // close connection
    close(clientfd);

    return 0;
}
