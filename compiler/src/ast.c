#include "ast.h"

#include <math.h>
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
                    print_tree(to_print->first_arg, out);
                    fprintf(out, " %s", op);
                    break;
                case COS:
                    op = "cos";
                    print_tree(to_print->first_arg, out);
                    fprintf(out, " %s", op);
                    break;
                case TAN:
                    op = "tan";
                    print_tree(to_print->first_arg, out);
                    fprintf(out, " %s", op);
                    break;
                case CTG:
                    op = "ctg";
                    print_tree(to_print->first_arg, out);
                    fprintf(out, " %s", op);
                    break;
                case PLUS:
                    op = "+";
                    print_tree(to_print->first_arg, out);
                    fputs(" ", out);
                    print_tree(to_print->second_arg, out);
                    fprintf(out, " %s", op);
                    break;
                case MINUS:
                    op = "-";
                    print_tree(to_print->first_arg, out);
                    fputs(" ", out);
                    print_tree(to_print->second_arg, out);
                    fprintf(out, " %s", op);
                    break;
                case MULTIPLY:
                    op = "*";
                    print_tree(to_print->first_arg, out);
                    fputs(" ", out);
                    print_tree(to_print->second_arg, out);
                    fprintf(out, " %s", op);
                    break;
                case DIVIDE:
                    op = "/";
                    print_tree(to_print->first_arg, out);
                    fputs(" ", out);
                    print_tree(to_print->second_arg, out);
                    fprintf(out, " %s", op);
                    break;
            }
            break;
    }
}

AST* copy_tree(AST* source) {
    AST *result = create_tree();
    result->node_type = source->node_type;
    if(source->node_type == CONSTANT) {
        result->value = source->value;
    }
    else if(source->node_type == EXPRESSION) {
        result->operator_type = source->operator_type;
        result->first_arg = copy_tree(source->first_arg);
        switch(source->operator_type) {
            case PLUS:
            case MINUS:
            case MULTIPLY:
            case DIVIDE:
                result->second_arg = copy_tree(source->second_arg);
                break;
        }
    }
    return result;
}

bool equals(AST* left, AST* right) {
    double accuracy = 0.000001;
    if(left->node_type != right->node_type) {
        return false;
    }
    switch(left->node_type) {
        case CONSTANT:
            if(fabs(left->value - right->value) >= accuracy) {
                return false;
            }
            break;
        case EXPRESSION:
            if(left->operator_type != right->operator_type) {
                return false;
            }
            if(!equals(left->first_arg, right->first_arg)) {
                return false;
            }
            switch(left->operator_type) {
                case PLUS:
                case MINUS:
                case MULTIPLY:
                case DIVIDE:
                    if(!equals(left->second_arg, right->second_arg)) {
                        return false;
                    }
            }
            break;
    }
    return true;
}
