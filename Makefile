SPEC_FILE ?= input.txt
YASMFLAGS = -g dwarf2 -DUNIX -felf64
BUILD_DIR = build
OUTPUT_DIR = out
GENERATED_ASM = output.asm

all: build

build: dir solver

run: build
	$(OUTPUT_DIR)/main

dir:
	mkdir -p $(OUTPUT_DIR)
	mkdir -p $(BUILD_DIR)

clean:
	rm -r $(OUTPUT_DIR)
	rm -r $(BUILD_DIR)

functions.o: compiler $(SPEC_FILE)
	$(OUTPUT_DIR)/compiler $(SPEC_FILE) $(OUTPUT_DIR)/$(GENERATED_ASM)
	yasm $(YASMFLAGS) $(OUTPUT_DIR)/$(GENERATED_ASM) -o $(BUILD_DIR)/functions.o

solver: functions.o
	make -C solver
	mv solver/solver $(OUTPUT_DIR)/main

compiler:
	make -C compiler
	mv compiler/compiler $(OUTPUT_DIR)/compiler

.PHONY: solver compiler all clean
