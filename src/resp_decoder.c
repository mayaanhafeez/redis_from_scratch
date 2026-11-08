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

static char** command_list;

//take in 1 input command and its length, change the global command_list, return 0 on success and -1 on failure
int parse_input(const char* input, size_t len) {
    unsigned int command_count;
    unsigned int i=0;
    if (input[0]!='*') return -1;
    while (input[i]!='\r') {
        i++;
    }

    char command_count_s[i];
    strncpy(command_count_s, input + 1, i-1);
    command_count_s[i]='\0';

    if (strcmp(command_count_s, "0")!=0) {
        command_count = 0;
    } else {
        command_count = atoi(command_count_s);
    }

    if (input[i+1]!='\n') return -1;
    command_list = malloc(command_count*sizeof(char*));
    if (command_list == NULL) {
        perror("malloc fail");
        exit(1);
    }

    return 0;
}

int main() {
    const char* dummy = "*2\r\n$4\r\nECHO\r\n$5\r\nhello\r\n";

    parse_input(dummy, strlen(dummy));
    return 0;
}
