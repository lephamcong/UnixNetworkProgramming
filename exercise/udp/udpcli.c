/* gcc udpcli.c -o udpcli
./udpcli 127.0.0.1
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SERV_PORT 5501
#define SA struct sockaddr_in
#define MAXLINE 1000

typedef struct {
    int id;
    char name[50];
    int age;
    int salary;
} Employee;

void handle_employee(FILE *FP, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    
    Employee employee;
    while(1){
        // Read employee data from stdin
        printf("Enter employee data...\n");

        printf("ID: ");
        scanf("%d", &employee.id);
        getchar();

        printf("Name: ");
        fgets(employee.name, sizeof(employee.name), FP);
        employee.name[strcspn(employee.name, "\n")] = 0;

        printf("Age: ");
        scanf("%d", &employee.age);
        getchar();

        printf("Salary: ");
        scanf("%d", &employee.salary);
        getchar();
        
        // Send to server
        if (sendto(sockfd, &employee,sizeof(employee),0,pservaddr,servlen) < 0) {
            perror("Write error");
            // close(sockfd);
            exit(1);
        } else {
            printf("Data sent to server\n");
            printf("-------------------\n");
            printf("ID: %d\n", employee.id);
            printf("Name: %s\n", employee.name);
            printf("Age: %d\n", employee.age);
            printf("Salary: %d\n", employee.salary);
            printf("-------------------\n");
        }
    }
}

int main(int argc, char ** argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2){
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    // memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET, argv[1], &servaddr.sin_addr.s_addr);
    printf("Set address successfully\n");

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sockfd == -1) {
        printf("Error creating socket");
        exit(1);
    }
    printf("Create socket successfully\n");

    handle_employee(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    
    return 0;
}