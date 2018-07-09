#include "derivative.h"

AST* derivative(AST* tree) {
    AST* result = create_tree();
    switch(tree->node_type) {
        case CONSTANT:
            result->node_type = CONSTANT;
            result->value = 0;
            break;
        case VARIABLE:
            result->node_type = CONSTANT;
            result->value = 1;
            break;
        case EXPRESSION:
            result->node_type = EXPRESSION;
            switch(tree->operator_type) {
                case PLUS:
                case MINUS:
                    result->operator_type = tree->operator_type;
                    result->first_arg = derivative(tree->first_arg);
                    result->second_arg = derivative(tree->second_arg);
                    break;
                case MULTIPLY:
                    result->operator_type = PLUS;
                    AST* laddend = create_tree();
                    AST* raddend = create_tree();
                    laddend->node_type = EXPRESSION;
                    laddend->operator_type = MULTIPLY;
                    laddend->first_arg = derivative(tree->first_arg);
                    laddend->second_arg = copy_tree(tree->second_arg);
                    raddend->node_type = EXPRESSION;
                    raddend->operator_type = MULTIPLY;
                    raddend->first_arg = copy_tree(tree->first_arg);
                    raddend->second_arg = derivative(tree->second_arg);
                    result->first_arg = laddend;
                    result->second_arg = raddend;
                    break;
                case DIVIDE:
                    result->operator_type = DIVIDE;
                    AST* nominator = create_tree();
                    AST* denominator = create_tree();
                    nominator->node_type = EXPRESSION;
                    nominator->operator_type = MINUS;
                    laddend = create_tree();
                    raddend = create_tree();
                    laddend->node_type = EXPRESSION;
                    laddend->operator_type = MULTIPLY;
                    laddend->first_arg = derivative(tree->first_arg);
                    laddend->second_arg = copy_tree(tree->second_arg);
                    raddend->node_type = EXPRESSION;
                    raddend->operator_type = MULTIPLY;
                    raddend->first_arg = copy_tree(tree->first_arg);
                    raddend->second_arg = derivative(tree->second_arg);
                    nominator->first_arg = laddend;
                    nominator->second_arg = raddend;
                    denominator->node_type = EXPRESSION;
                    denominator->operator_type = MULTIPLY;
                    denominator->first_arg = copy_tree(tree->second_arg);
                    denominator->second_arg = copy_tree(tree->second_arg);
                    result->first_arg = nominator;
                    result->second_arg = denominator;
                    break;
             case SIN:
					result->operator_type = MULTIPLY;
					AST* leftarg = create_tree();
					AST* rightarg = derivative(tree->first_arg);
					leftarg->node_type = EXPRESSION;
					leftarg->operator_type = COS;
					leftarg->first_arg = copy_tree(tree->first_arg);
					result->first_arg = leftarg;
					result->second_arg = rightarg;
					break;
				case COS:
					result->operator_type = MINUS;
					leftarg = create_tree();
					rightarg = create_tree();
					leftarg->node_type = CONSTANT;
					leftarg->value = 0;
					rightarg->node_type = EXPRESSION;
					rightarg->operator_type = MULTIPLY;
					AST* rlarg = create_tree();
					AST* rrarg = derivative(tree->first_arg);
					rlarg->node_type = EXPRESSION;
					rlarg->operator_type = SIN;
					rlarg->first_arg = copy_tree(tree->first_arg);
					rightarg->first_arg = rlarg;
					rightarg->second_arg = rrarg;
					result->first_arg = leftarg;
					result->second_arg = rightarg;
					break;
				case TAN:
					result->operator_type = DIVIDE;
					rightarg = create_tree();
					rlarg = create_tree();
					rlarg->node_type = EXPRESSION;
					rlarg->operator_type = COS;
					rlarg->first_arg = copy_tree(tree->first_arg);
					rightarg->node_type = EXPRESSION;
					rightarg->operator_type = MULTIPLY;
					rightarg->first_arg = rlarg;
					rightarg->second_arg = copy_tree(rlarg);
					result->first_arg = derivative(tree->first_arg);
					result->second_arg = rightarg;
					break;
				case CTG:
					result->operator_type = MINUS;
					leftarg = create_tree();
					rightarg = create_tree();
					leftarg->node_type = CONSTANT;
					leftarg->value = 0;
					rightarg->node_type = EXPRESSION;
					rightarg->operator_type = DIVIDE;
					rrarg = create_tree();
					rlarg = create_tree(); //In fact rrlarg, but there are too many variables.
					rlarg->node_type = EXPRESSION;
					rlarg->operator_type = SIN;
					rlarg->first_arg = copy_tree(tree->first_arg);
					rrarg->node_type = EXPRESSION;
					rrarg->operator_type = MULTIPLY;
					rrarg->first_arg = rlarg;
					rrarg->second_arg = copy_tree(rlarg);
					rightarg->first_arg = derivative(tree->first_arg);
					rightarg->second_arg = rrarg;
					result->first_arg = leftarg;
					result->second_arg = rightarg;
					break;
            }
            break;
    }
    return result;
}
