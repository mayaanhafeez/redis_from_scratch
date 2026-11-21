#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int create_listener() {
    struct addrinfo hints;
    struct addrinfo *res;
    int sockfd;
    int new_fd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //v4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(NULL, "6767", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        free(res);
        perror("socket");
        return -1;
    }

    int yes = 1;
    status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    if (status == -1) {
        perror("setsockopt");
        free(res);
        return -1;
    }

    status = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (status != 0) {
        perror("bind");
        free(res);
        return -1;
    }
    listen(sockfd, 10);
    return sockfd;
}

int serve_client(int fd) {
    char* msg = "*3\r\n$4\r\nECHO\r\n$5\r\nhello\r\n$3\r\nkys\r\n";
    int bytes_sent = send(fd, msg, strlen(msg), 0);
    if (bytes_sent == 0) {
        fprintf(stderr, "bytes not sent\n");
    } else {
        printf("sent %d bytes\n", bytes_sent);
    }
    char buf[4096];
    while (1) {
        recv(fd, buf, 4095, 0);
        // printf("%s\n", buf);
    }
    return 0;
}


int main() {
    int fd;
    fd = create_listener();
    if (fd == -1) {
        fprintf(stderr, "could not create listner\n");
        exit(1);
    }

    int status = accept(fd, NULL, NULL);
    if (status == -1) {
        perror("accept");
        exit(1);
    }

    serve_client(status);
    return 0;
}

