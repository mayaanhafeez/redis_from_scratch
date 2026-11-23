#include <stddef.h>
#include "resp.h"

#ifndef COMMANDS_H

#define COMMANDS_H

int execute_command(struct Command *input_command, struct Output *output);

#endif
