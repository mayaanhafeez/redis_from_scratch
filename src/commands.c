// #include "resp.h"
//
// struct command_functions {
//     char* command;
//     int (*fp)();
// };
//
//
// int simple_string_encoder(char* input_string, struct Output *output);
//
// int error_encoder(char* input_error, struct Output *output);
//
// int array_encoder(size_t n, char** output);
//
// int integer_encoder(long long input_int, struct Output *output);
//
// int bulk_string_encoder(char* input_string, size_t len, struct Output *output);
//
// static const command_table[];
//
// int execute(struct Command input_command, char** output, size_t* output_len) {
//
// }
