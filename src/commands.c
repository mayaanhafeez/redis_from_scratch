#include "resp.h"
#include <stdio.h>
#include <string.h>

struct command_functions {
    char* command;
    int (*fp)(struct Command *command, struct Output *output);
};

int echo_handler(struct Command *command, struct Output *output) {
    if (command->argc == 2) {
        int ret = bulk_string_encoder(command->args[1].command,command->args[1].size , output);
        if (ret != 0) {
            fprintf(stderr, "bad stuff");
        }
        return ret;
    }
    else if (command->argc > 2){
        int ret = error_encoder("ERR wrong number of arguments", output);
        if (ret != 0) {
            fprintf(stderr, "bad stuff");
        }
        return ret;
    } else {
        return -1;
    }
}

int ping_handler(struct Command *command, struct Output *output){
    if (command->argc == 1) {
        int ret = simple_string_encoder("PONG", output);
        if (ret != 0) {
            fprintf(stderr, "something bad");
        }
        return ret;
    } else {
        int ret = echo_handler(command, output);
        if (ret != 0) {
            fprintf(stderr, "something bad");
        }
        return ret;
    }
}

int command_handler(struct Command *command, struct Output *output) {
    (void)command;
    int ret = array_encoder(output, 0);
    return ret;
}

static const struct command_functions command_table[] = {
    {"ECHO", echo_handler},
    {"PING", ping_handler},
    {"COMMAND", command_handler},
};

int execute_command(struct Command *input_command, struct Output *output) {
    for (int i = 0; i<3; i++) {
        if (strcmp(input_command->args[1].command, command_table[i].command)==0) {
            command_table[i].fp(input_command, output);
        }
    }
    return 1;
}


