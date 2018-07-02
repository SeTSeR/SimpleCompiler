
CFLAGS = -g -Iinclude -c -std=gnu99
LDFLAGS =
SRC_DIR = src
BUILD_DIR = ../build/solver
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
EXE = solver

all: dir $(EXE)

$(EXE): $(OBJ) $(BUILD_DIR)/../functions.o
	$(CC) $^ -o $@ $(LDFLAGS)

dir:
	mkdir -p $(BUILD_DIR)

$(OBJ): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@
