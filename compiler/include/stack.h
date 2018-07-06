#include "ast.h"

typedef struct stack stack;

void clear(stack* st);
void push(stack* st, AST* tree);
AST* pop(stack* st);
int size(stack* st);

stack* create_stack();
void destroy_stack(stack* st);
