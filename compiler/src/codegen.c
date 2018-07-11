#include "codegen.h"
#include "hashset.h"

void gen_header(FILE* out) {
    fputs("[BITS 64]\n", out);
    fputs("\n", out);
    fputs("global a\n", out);
    fputs("global b\n", out);
    fputs("\n", out);
    fputs("global f1\n", out);
    fputs("global f2\n", out);
    fputs("global f3\n", out);
    fputs("global df1\n", out);
    fputs("global df2\n", out);
    fputs("global df3\n", out);
    fputs("\n", out);
}

hashset* collect_node(AST* function) {
    hashset *result = create_hashset();
    switch(function->node_type) {
        case CONSTANT:
            insert(result, function->value);
            break;
        case EXPRESSION:
            append(result, collect_node(function->first_arg));
            switch(function->operator_type) {
                case PLUS:
                case MINUS:
                case MULTIPLY:
                case DIVIDE:
                    append(result, collect_node(function->second_arg));
                    break;
            }
    }
    return result;
}

hashset* collect_info(AST* functions[], AST* derivatives[]) {
    hashset* result = create_hashset();
    for(int i = 0; i < 3; ++i) {
        hashset* appended = collect_node(functions[i]);
        append(result, appended);
        destroy_hashset(appended);
        appended = collect_node(derivatives[i]);
        append(result, appended);
        destroy_hashset(appended);
    }
    return result;
}

void gen_rodata(double** constants, size_t* size, double a, double b, AST* functions[], AST* derivatives[], FILE* out) {
    fputs("section .rodata\n", out);
    fprintf(out, "    a dq %lf\n", a);
    fprintf(out, "    b dq %lf\n", b);
    hashset* table = collect_info(functions, derivatives);
    *constants = to_array(table, size);
    for(size_t i = 0; i < *size; ++i) {
        fprintf(out, "    const%d dq %lf\n", i + 1, (*constants)[i]);
    }
}

void gen_text(double* constants, int size, AST* functions[], AST* derivatives[], FILE* out) {
}

void gen_listing(double a, double b, AST* functions[], AST* derivatives[], FILE* out) {
    gen_header(out);
    double* constants;
    size_t size;
    gen_rodata(&constants, &size, a, b, functions, derivatives, out);
    gen_text(constants, size, functions, derivatives, out);
}
