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

char* simple_string_encoder(struct SimpleString input_string) {
    char* output = malloc((input_string.size+3)*sizeof(char));
    if (output == NULL) {
        perror("malloc fail");
        exit(1);
    }
    output[0] = '+';
    memcpy(output + 1, input_string.string, input_string.size);
    output[input_string.size+1] = '\r';
    output[input_string.size+2] = '\n';
    return output;
}

int main() {
    const char* dummy = "*3\r\n$4\r\nECHO\r\n$5\r\nhello\r\n$3\r\nkys\r\n";
    struct Command command;

    int value = parse_input(&command, dummy, strlen(dummy));
    printf("return value:%d\n",value);
    free_command_list(command.args, command.argc);
    struct SimpleString input_string;
    input_string.string = "hawk";
    input_string.size = 4;

    char * stored_string = simple_string_encoder(input_string);
    printf("%s", stored_string);
    printf("hello");
    return 0;
}
