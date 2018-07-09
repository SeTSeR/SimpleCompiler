#include "optimizer.h"

#include <math.h>

void fold_constants(AST* tree) {
    if(tree->node_type == EXPRESSION) {
            fold_constants(tree->first_arg);
            if(tree->first_arg->node_type == CONSTANT) {
                switch(tree->operator_type) {
                    case SIN:
                        tree->node_type = CONSTANT;
                        tree->value = sin(tree->first_arg->value);
                        destroy_tree(tree->first_arg);
                        break;
                    case COS:
                        tree->node_type = CONSTANT;
                        tree->value = cos(tree->first_arg->value);
                        destroy_tree(tree->first_arg);
                        break;
                    case TAN:
                        tree->node_type = CONSTANT;
                        tree->value = tan(tree->first_arg->value);
                        destroy_tree(tree->first_arg);
                        break;
                    case CTG:
                        tree->node_type = CONSTANT;
                        tree->value = 1/tan(tree->first_arg->value);
                        destroy_tree(tree->first_arg);
                        break;
                    case PLUS:
                        fold_constants(tree->second_arg);
                        if(tree->second_arg->node_type == CONSTANT) {
                            tree->node_type = CONSTANT;
                            tree->value = tree->first_arg->value + tree->second_arg->value;
                        }
                        break;
                    case MINUS:
                        fold_constants(tree->second_arg);
                        if(tree->second_arg->node_type == CONSTANT) {
                            tree->node_type = CONSTANT;
                            tree->value = tree->first_arg->value - tree->second_arg->value;
                        }
                        break;
                    case MULTIPLY:
                        fold_constants(tree->second_arg);
                        if(tree->second_arg->node_type == CONSTANT) {
                            tree->node_type = CONSTANT;
                            tree->value = tree->first_arg->value * tree->second_arg->value;
                        }
                        break;
                    case DIVIDE:
                        fold_constants(tree->second_arg);
                        if(tree->second_arg->node_type == CONSTANT) {
                            tree->node_type = CONSTANT;
                            tree->value = tree->first_arg->value + tree->second_arg->value;
                        }
                        break;
                }
            }
        }
}

void mathematic_optimizer(AST* tree) {
    double accuracy = 0.000001;
        if(tree->node_type == EXPRESSION) {
            mathematic_optimizer(tree->first_arg);
            switch(tree->operator_type) {
                case PLUS:
                    mathematic_optimizer(tree->second_arg);
                    if((tree->second_arg->node_type == CONSTANT) &&
                            (fabs(tree->second_arg->value) < accuracy)) {
                        destroy_tree(tree->second_arg);
                        move_tree(tree->first_arg, tree);
                    }
                    break;
                case MINUS:
                    mathematic_optimizer(tree->second_arg);
                    if((tree->second_arg->node_type == CONSTANT) &&
                            (fabs(tree->second_arg->value) < accuracy)) {
                        destroy_tree(tree->second_arg);
                        move_tree(tree->first_arg, tree);
                    }
                    break;
                case MULTIPLY:
                    mathematic_optimizer(tree->second_arg);
                    if((tree->first_arg->node_type == CONSTANT) &&
                            (fabs(tree->first_arg->value) < accuracy)) {
                        destroy_tree(tree->first_arg);
                        destroy_tree(tree->second_arg);
                        tree->node_type = CONSTANT;
                        tree->value = 0;
                    }
                    break;
                case DIVIDE:
                    mathematic_optimizer(tree->second_arg);
                    if((tree->first_arg->node_type == CONSTANT) &&
                            (fabs(tree->first_arg->value) < accuracy)) {
                        destroy_tree(tree->first_arg);
                        destroy_tree(tree->second_arg);
                        tree->node_type = CONSTANT;
                        tree->value = 0;
                    }
                    break;
            }
        }
}

void optimize(AST* tree) {
    mathematic_optimizer(tree);
    fold_constants(tree);
}
