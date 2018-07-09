#ifndef _AST_H
#define _AST_H

#include <stdio.h>

enum NODE_TYPE {
    VARIABLE,
    CONSTANT,
    EXPRESSION
};

enum OPERATOR_TYPE {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    SIN,
    COS,
    TAN,
    CTG
};

typedef enum NODE_TYPE NODE_TYPE;
typedef enum OPERATOR_TYPE OPERATOR_TYPE;

struct AST {
    NODE_TYPE node_type;

    double value;

    OPERATOR_TYPE operator_type;
    struct AST *first_arg;
    struct AST *second_arg;
};

typedef struct AST AST;

AST *create_tree();

void destroy_tree(AST*);
void print_tree(AST*, FILE*);

AST* copy_tree(AST*);

#endif
