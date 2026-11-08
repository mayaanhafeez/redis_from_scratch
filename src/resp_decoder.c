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
    printf("%d", command_count);
    command_list = malloc(sizeof(struct Command));

    i++;
    if (input[i]!='\n') return -1;
    i++;
    while (i<len) {
        if (input[i]!='$') return -1;
        unsigned int str_len = 0;
        while (input[i]!='\r') {
            if (input[i] > '9' || input[i] < '0') return -1;
            str_len = (str_len*10) + (input[i] - '0');
            i++;
        }
        char command; //continue
    }


    return 0;
}

int main() {
    const char* dummy = "*2\r\n$4\r\nECHO\r\n$5\r\nhello\r\n";

    parse_input(dummy, strlen(dummy));
    return 0;
}
