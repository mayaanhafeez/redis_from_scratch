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

struct Output {
    char* string;
    size_t size;
};


int parse_input(struct Command *command, const char* input, size_t);

int simple_string_encoder(char* input_string, struct Output *output);

int error_encoder(char* input_error, struct Output *output);

int array_encoder(struct Output *output, size_t n);

int integer_encoder(long long input_int, struct Output *output);

int bulk_string_encoder(char* input_string, size_t len, struct Output *output);

#endif
