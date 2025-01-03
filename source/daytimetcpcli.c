#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for close()
#include <sys/types.h> // for socket()
#include <sys/socket.h> // for socket()
#include <netinet/in.h> // for sockaddr_in
#include <arpa/inet.h> // for inet_pton()

#define MAXLINE 100

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char recvline[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }

    // create TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // create server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8855);  // Daytime server port
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        exit(1);
    }

    // connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // read from server
    ssize_t n = read(sockfd, recvline, MAXLINE);
    if (n > 0) {
        recvline[n] = 0;  // null terminate
        fputs(recvline, stdout);  // write to stdout
    } else {
        perror("Read error");
    }

    // close socket
    close(sockfd);
    return 0;
}
