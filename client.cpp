#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int sockfd;
int received_length;

void connect(void) {
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(1234);

    int result = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if (result < 0) {
        printf("Connect error\n");
        exit(0);
    }
}

int get_received_length(void) {
    return received_length;
}

unsigned char* receive(unsigned int timeout) {
    int got_so_far = 0;
    int result;

    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));

    result = recv(sockfd, &received_length, 4, 0);

    if (result < 0) {
        if (errno == EAGAIN) {
            received_length = 0;
            return NULL;
        }

        printf("Receive error%i\n", result);
        exit(0);
    }

    received_length = ntohl(received_length);
    unsigned char *packet = 
        (unsigned char*)malloc(sizeof(unsigned char) * received_length);

    while (got_so_far < received_length) {
        result = recv(sockfd, 
            packet + got_so_far, 
            received_length - got_so_far, 0);

        if (result < 0) {
            printf("Receive error\n");
            exit(0);
        }

        got_so_far += result;
    }

    return packet;
}

void send(unsigned char* data, int length) {
    int result;

    int plength = htonl(length);

    result = send(sockfd, &plength, 4, 0);

    if (result < 0) {
        printf("Send error\n");
        exit(0);
    }

    result = send(sockfd, data, length, 0);

    if (result < 0) {
        printf("Send error\n");
        exit(0);
    }
}
