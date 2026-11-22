#include <stddef.h>

#ifndef RESP_H
#define RESP_H

struct Arg {
    char* command;
    size_t size;
};

struct Command {
    struct Arg *args;
    size_t argc;
};

struct String {
    char* string;
    size_t size;
};

struct Error {
    char* string;
    size_t size;
};

struct Integer {
    char* value;
    size_t size;
};


int parse_input(struct Command *command, const char* input, size_t);

int simple_string_encoder(char* input_string, struct String *output);

int error_encoder(char* input_error, struct Error *output);

int array_encoder(size_t n, char** output);

int integer_encoder(long long input_int, struct Integer *output);

int bulk_string_encoder(char* input_string, size_t len, struct String *output);

#endif
