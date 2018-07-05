CFLAGS = -g -Iinclude -c -std=gnu99
LDFLAGS = -lm
SRC_DIR = src
BUILD_DIR = ../build/compiler
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
EXE = compiler

all: dir $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

dir:
	mkdir -p $(BUILD_DIR)

$(OBJ): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -r $(BUILD_DIR)
	rm $(EXE)

PHONY: dir clean
