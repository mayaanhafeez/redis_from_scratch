#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// example input: *2\r\n$4\r\nECHO\r\n$5\r\nhello\r\n
// example output: ECHO hello

enum resp_data_type {
    string,
    integer,
    error,
    bulk_string,
    array,
};

struct Command {
    char* com;
    size_t size;
};

static struct Command* command_list;

//take in 1 input command and its length, change the global command_list, return 0 on success and -1 on failure
int parse_input(const char* input, size_t len) {
    unsigned int command_count = 0;
    unsigned int i=0;
    if (input[0]!='*') return -1;
    i++;
    while (input[i]!='\r') {
        if (input[i] > '9' || input[i] < '0') return -1;
        command_count = (command_count*10) + (input[i] - '0');
        i++;
    }
    command_list = malloc(sizeof(struct Command));

    if (command_list == NULL) {
        perror("malloc fail");
        exit(1);
    }

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
        char* command = malloc(str_len*sizeof(char));
        memcpy(command, input + i, str_len); //continue
        command[str_len] = '\0';
        i = i + str_len + 2;
        printf("current char: %c, i= %d\n", input[i], i);
        printf("command: %s\n", command);

    }

    return 0;
}

int main() {
    const char* dummy = "*3\r\n$4\r\nECHO\r\n$5\r\nhello\r\n$3\r\nkys\r\n";

    parse_input(dummy, strlen(dummy));
    return 0;
}
