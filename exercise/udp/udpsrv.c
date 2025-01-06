/* gcc udpsrv.c -o udpsrv
./udpsrv
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SERV_PORT 5501
#define SA struct sockaddr
#define MAXLINE 1000

typedef struct {
    int id;
    char name[50];
    int age;
    int salary;
} Employee;

void echo_employee(int sockfd, struct sockaddr * pcliaddr, socklen_t clilen);

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in serveraddr, cliaddr;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sockfd == -1) {
        printf("Error creating socket");
        exit(1);
    }
    printf("Create socket successfully\n");

    bzero(&serveraddr, sizeof(serveraddr));
    // memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERV_PORT);
    printf("Set address successfully\n");

    if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 )
    {
        // exit(1);
    }
    printf("Bind successfully\n");
    
    echo_employee(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    return 0;
}


void echo_employee(int sockfd, struct sockaddr * pcliaddr, socklen_t clilen){
    int n;
    socklen_t len;
    Employee employee;

    while(1){
        len = clilen;
        // Receive from client
        n = recvfrom(sockfd, &employee, sizeof(employee), 0, pcliaddr, &len);
        printf(" Received %d bytes\n", n);
        printf("Received Employee Data:\n");
        printf("ID: %d\n", employee.id);
        printf("Name: %s\n", employee.name);
        printf("Age: %d\n", employee.age);
        printf("Age: %d\n", employee.salary);
    }
}