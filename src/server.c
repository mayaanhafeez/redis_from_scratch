#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "resp.h"
#include "commands.h"

int create_listener() {
    struct addrinfo hints;
    struct addrinfo *res;
    int sockfd;

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
    char* msg;
    char buf[16384];
    int bytes_recv;
    int bytes_sent;
    while (1) {
        bytes_recv = recv(fd, buf, 16384, 0);
        if (bytes_recv < 0) {
            fprintf(stderr, "things broke bro");
        } else if (bytes_recv == 0) {
            exit(1);
        } else {
            struct Command input_command;
            struct Output output;
            parse_input(&input_command, buf, bytes_recv);
            printf("bytes recv: %d\n", bytes_recv);
            printf("command: %s\n", input_command.args->command);
            execute_command(&input_command, &output);
            msg = output.string;
            printf("msg:%s\n", output.string);
            bytes_sent = send(fd, msg, output.size, 0);
            if (bytes_sent == 0) {
                fprintf(stderr, "bytes not sent\n");
            } else {
                printf("sent %d bytes\n", bytes_sent);
            }
            free_command_list(input_command.args, input_command.argc);
        }
    }
    return 1;
}


int main() {
    int fd;
    fd = create_listener();
    if (fd == -1) {
        fprintf(stderr, "could not create listner\n");
        exit(1);
    }

    int new_fd = accept(fd, NULL, NULL);
    if (new_fd == -1) {
        perror("accept");
        exit(1);
    }

    serve_client(new_fd);
    return 0;
}

