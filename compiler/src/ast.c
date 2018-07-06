#include "ast.h"

#include <stdlib.h>

AST* create_tree() {
    return malloc(sizeof(AST));
}

void destroy_tree(AST *to_destroy) {
    if(to_destroy) {
        if(to_destroy->node_type != EXPRESSION) {
            free(to_destroy);
        }
        else {
            switch(to_destroy->operator_type) {
                case PLUS:
                case MINUS:
                case MULTIPLY:
                case DIVIDE:
                    destroy_tree(to_destroy->second_arg);
                case SIN:
                case COS:
                case TAN:
                case CTG:
                    destroy_tree(to_destroy->first_arg);
                    free(to_destroy);
                    break;
            }
        }
    }
}

void print_tree(AST* to_print, FILE* out) {
    char* op;
    switch(to_print->node_type) {
        case VARIABLE:
            fputs("x", out);
            break;
        case CONSTANT:
            fprintf(out, "%lf", to_print->value);
            break;
        case EXPRESSION:
            switch(to_print->operator_type) {
                case SIN:
                    op = "sin";
                case COS:
                    op = "cos";
                case TAN:
                    op = "tan";
                case CTG:
                    op = "ctg";
                    print_tree(to_print->first_arg, out);
                    fprintf(out, " %s", op);
                    break;
                case PLUS:
                    op = "+";
                case MINUS:
                    op = "-";
                case MULTIPLY:
                    op = "*";
                case DIVIDE:
                    op = "/";
                    print_tree(to_print->first_arg, out);
                    fputs(" ", out);
                    print_tree(to_print->second_arg, out);
                    fprintf(out, " %s", op);
            }
            break;
    }
}
