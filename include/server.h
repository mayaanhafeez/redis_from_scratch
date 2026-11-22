#import <stddef.h>

#ifndef SERVER_H
#define SERVER_H

int serve_client(int fd);

int create_listener();

#endif
