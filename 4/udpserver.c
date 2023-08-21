#include <sys/socket.h>
#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int sd;
    char buff[1024];
    struct sockaddr_in cliaddr, servaddr;
    socklen_t clilen;
    clilen = sizeof(cliaddr);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("Cannot open Socket");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(5669);

    if (bind(sd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error in binding the port");
        exit(1);
    }

    printf("Server is Running...\n");

    while (1) {
        bzero(buff, sizeof(buff));

        if (recvfrom(sd, buff, sizeof(buff), 0, (struct sockaddr*)&cliaddr, &clilen) < 0) {
            perror("Cannot receive data");
            exit(1);
        }
        printf("Message is received: %s\n", buff);

        if (sendto(sd, buff, strlen(buff), 0, (struct sockaddr*)&cliaddr, clilen) < 0) {
            perror("Cannot send data to client");
            exit(1);
        }
        printf("Sent data to UDP Client: %s\n", buff);
    }

    close(sd);
    return 0;
}
