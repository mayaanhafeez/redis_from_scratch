#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

enum resp_data_type {
    string,
    integer,
    error,
    bulk_string,
    array,
};

struct SimpleString {
    char* string;
    size_t size;
};

struct Arg {
    char* command;
    size_t size;
};

struct Command {
    struct Arg *args;
    size_t argc;
};

struct Integer {
    char* value;
    size_t size;
};

void free_command_list(struct Arg* command_list, size_t size) {
    for (size_t i=0; i< size; i++) {
        free(command_list[i].command);
    }
    free(command_list);
}

//take in 1 input command and its length, change the global command_list, return 0 on success and -1 on failure
int parse_input(struct Command *command, const char* input, size_t len) {
    struct Arg* command_list;
    unsigned int command_count = 0;
    unsigned int i=0;
    if (input[0]!='*') return -1;
    i++;
    while (input[i]!='\r') {
        if (input[i] > '9' || input[i] < '0') return -1;
        command_count = (command_count*10) + (input[i] - '0');
        i++;
    }
    unsigned int command_index = 0;
    command_list = malloc(command_count*sizeof(struct Arg));


    if (command_list == NULL) {
        perror("malloc fail");
        exit(1);
    }

    command->args = command_list;
    command->argc = (size_t)command_count;

    i++;
    if (input[i]!='\n') return -1;
    i++;

    while (i<len) {
        printf("current char: %c\n", input[i]);
        if (input[i]!='$') return -1;
        unsigned int str_len = 0;
        i++;
        while (input[i]!='\r') {
            printf("current char: %c\n", input[i]);
            if (input[i] > '9' || input[i] < '0') return -1;
            str_len = (str_len*10) + (input[i] - '0');
            printf("%d\n", str_len);
            i++;
        }

        i++;
        printf("current char: %c\n", input[i]);
        if (input[i] != '\n') return -1;
        i++;
        char* command = malloc((str_len+1)*sizeof(char));
        if (command == NULL) {
            perror("malloc fail");
            exit(1);
        }
        memcpy(command, input + i, str_len);
        command[str_len] = '\0';
        i = i + str_len + 2;
        printf("current char: %c, i= %d\n", input[i], i);

        command_list[command_index].command = command;
        command_list[command_index].size = str_len + 1;
        command_index++;
        printf("command: %s\n", command);
    }
    return 0;
}

int simple_string_encoder(char* input_string, struct SimpleString *output) {
    size_t size = strlen(input_string);
    output->string = malloc((size+3)*sizeof(char));
    output->size = size;

    if (output->string == NULL) {
        perror("malloc fail");
        exit(1);
    }

    output->string[0] = '+';
    memcpy(output->string + 1, input_string, size);
    output->string[size+1] = '\r';
    output->string[size+2] = '\n';

    return 0;
}

int integer_encoder(long long input_int, struct Integer output) {
    char buf[32];

    buf[0] = ':';
    int len = snprintf(buf + 1, sizeof(buf) -1, "%lld", input_int);
    char* int_s = malloc((len+3)*sizeof(char));

    if (int_s == NULL) {
        perror("malloc fail");
        exit(1);
    }

    output.value = int_s;
    output.size = len + 3;

    return 0;
}

int main() {
    const char* dummy = "*3\r\n$4\r\nECHO\r\n$5\r\nhello\r\n$3\r\nkys\r\n";
    struct Command command;

    int value = parse_input(&command, dummy, strlen(dummy));
    printf("return value:%d\n",value);
    free_command_list(command.args, command.argc);
    struct SimpleString simple_string;
    char* string = "hawk";

    simple_string_encoder(string, &simple_string);

    printf("%s", simple_string.string);
    printf("%zu\n", simple_string.size);
    return 0;
}
