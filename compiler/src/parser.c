#include "error.h"
#include "parser.h"
#include "stack.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

AST* parse(char* line) {
    stack* st = create_stack();
    char* token = strtok(line, " \n");
    while(token) {
        if(token[0]) {
            AST* newtree = create_tree();
            if(isalpha(token[0])) {
                if((token[0] == 'x') && (token[1] == '\0')) {
                    newtree->node_type = VARIABLE;
                }
                else if((token[0] == 'e') && (token[1] == '\0')) {
                    newtree->node_type = CONSTANT;
                    newtree->value = M_E;
                }
                else if(strncmp(token, "pi", 2) == 0) {
                    newtree->node_type = CONSTANT;
                    newtree->value = M_PI;
                }
                else {
                    newtree->node_type = EXPRESSION;
                    if(size(st) == 0) error("Ill-formed expression.");
                    newtree->first_arg = pop(st);
                    if(strncmp(token, "sin", 3) == 0) {
                        newtree->operator_type = SIN;
                    }
                    else if(strncmp(token, "cos", 3) == 0) {
                        newtree->operator_type = COS;
                    }
                    else if(strncmp(token, "tan", 3) == 0) {
                        newtree->operator_type = TAN;
                    }
                    else if(strncmp(token, "ctg", 3) == 0) {
                        newtree->operator_type = CTG;
                    }
                }
            }
            else if(isdigit(token[0])) {
                newtree->node_type = CONSTANT;
                sscanf(token, "%lf", &(newtree->value));
            }
            else {
                AST *first, *second;
                if(size(st) < 2) error("Ill-formed expression.");
                second = pop(st);
                first = pop(st);
                newtree->node_type = EXPRESSION;
                newtree->first_arg = first;
                newtree->second_arg = second;                
                switch(token[0]) {
                    case '+':
                        newtree->operator_type = PLUS;
                        break;
                    case '-':
                        newtree->operator_type = MINUS;
                        break;
                    case '*':
                        newtree->operator_type = MULTIPLY;
                        break;
                    case '/':
                        newtree->operator_type = DIVIDE;
                        break;
                    default:
                        error("Unknown token.");
                        break;
                }
            }
            push(st, newtree);
        }
        token = strtok(NULL, " \n");
    }
    if(size(st) != 1) error("Ill-formed expression.");
    AST* result = pop(st);
    destroy_stack(st);
    return result;
}
