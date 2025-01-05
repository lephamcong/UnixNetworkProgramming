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

int main() {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    time_t ticks;

    // create TCP socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // create server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // listen on all interfaces
    servaddr.sin_port = htons(8855);  // Daytime server port

    // bind server address to socket
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // listen for incoming connections
    if (listen(listenfd, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is running...\n");

    // accept and handle connections
    for (;;) {
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);  
        if (connfd < 0) {
            perror("Accept failed");
            continue;
        }

        // get current time
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));  

        // send current time to client
        if (write(connfd, buff, strlen(buff)) < 0) {
            perror("Write failed");
        }

        // close connection
        close(connfd);
    }

    close(listenfd);
    return 0;
}
