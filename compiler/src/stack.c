#include "stack.h"

#include <stdlib.h>

struct stack {
    AST** stack;
    int size;
    int capacity;
};

enum {
	INITIAL_CAPACITY = 10
};

stack *create_stack() {
	stack *stack = calloc(1, sizeof(stack));
	stack->size = 0;
	stack->capacity = INITIAL_CAPACITY;
	stack->stack = malloc(stack->capacity * sizeof(AST*));
    return stack;
}

void destroy_stack(stack *stack) {
	stack->size = 0;
	stack->capacity = 0;
	free(stack->stack);
	free(stack);
}

void clear(stack *st) {
	st->size = 0;
	st->capacity = INITIAL_CAPACITY;
	st->stack = realloc(st->stack, st->capacity * sizeof(AST*));
}

void push(stack *st, AST *tree) {
	if(st->size == st->capacity) {
		st->capacity <<= 1;
		st->stack = realloc(st->stack, st->capacity * sizeof(AST*));
	}
	st->stack[st->size++] = tree;
}

AST* pop(stack *st) {
	if(st->size <= 0) return NULL;
	return st->stack[--st->size];
}

int size(stack *st) {
	return st->size;
}
