#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for close()
#include <time.h> // for ctime()
#include <sys/types.h> // for socket(), bind(), listen(), accept(), write()
#include <sys/socket.h> // for socket(), bind(), listen(), accept(), write()
#include <netinet/in.h> // for struct sockaddr_in, htons(), htonl(), INADDR_ANY
#include <arpa/inet.h> // for inet_ntop()

#define MAXLINE 100

// echo server
int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char mesg[MAXLINE];
    time_t ticks;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(32000);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    for (;;) {
        len = sizeof(cliaddr);
        recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        printf("connection from %s, port %d\n",
               inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
               ntohs(cliaddr.sin_port));
        ticks = time(NULL);
        snprintf(mesg, sizeof(mesg), "%.24s\r\n", ctime(&ticks));
        sendto(sockfd, mesg, strlen(mesg), 0, (struct sockaddr *)&cliaddr, len);
    }
}