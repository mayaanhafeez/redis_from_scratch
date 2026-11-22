CC      := cc
CFLAGS  := -Wall -Wextra -g -Iinclude -MMD -MP
LDFLAGS :=

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)
BIN := redis

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

asan: CFLAGS  += -fsanitize=address
asan: LDFLAGS += -fsanitize=address
asan: clean all

clean:
	rm -rf build $(BIN)

-include $(OBJ:.o=.d)

.PHONY: all asan clean
