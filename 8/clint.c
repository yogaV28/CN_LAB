#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]) {
    int sd, cd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    char buff[100], file[10000];
    struct hostent *h;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_hostname>\n", argv[0]);
        return 1;
    }

    h = gethostbyname(argv[1]);
    if (h == NULL) {
        perror("gethostbyname");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = h->h_addrtype;
    memcpy((char*)&servaddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    servaddr.sin_port = htons(1500);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    cd = connect(sd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    while (1) {
        printf("%s\n", "Enter the File Name:");
        scanf("%s", buff);
        send(sd, buff, strlen(buff) + 1, 0);

        printf("%s\n", "File Output:");
        ssize_t bytes_received = recv(sd, file, sizeof(file), 0);

        if (bytes_received < 0) {
            perror("recv");
            break;
        } else if (bytes_received == 0) {
            printf("Connection closed by server.\n");
            break;
        } else {
            printf("%s", file);
        }
    }

    close(sd); // Close the socket when done

    return 0;
}
