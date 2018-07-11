#include "codegen.h"
#include "hashset.h"

#include <math.h>
#include <stdlib.h>

#define ACCURACY 0.00001

void gen_header(FILE* out) {
    fputs("[BITS 64]\n", out);
    fputs("default rel\n", out);
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
    destroy_hashset(table);
    for(size_t i = 0; i < *size; ++i) {
        fprintf(out, "    const%d dq %lf\n", i + 1, (*constants)[i]);
    }
    fputs("\n", out);
}

void gen_prologue(FILE* out) {
    fputs("    movsd qword[rsp - 8], xmm0\n", out);
}

void gen_epilogue(FILE* out) {
    fputs("    fstp qword[rsp - 8]\n", out);
    fputs("    movsd xmm0, qword[rsp - 8]\n", out);
    fputs("    ret\n", out);
}

void gen_node(double* constants, size_t size, AST* function, FILE* out) {
    size_t j = -1;
    switch(function->node_type) {
        case CONSTANT:
            for(size_t i = 0; i < size; ++i) {
                if(fabs(function->value - constants[i]) < ACCURACY) {
                    j = i;
                    break;
                }
            }
            fprintf(out, "    fld qword[const%d]\n", j + 1);
            break;
        case VARIABLE:
            fputs("    fld qword[rsp - 8]\n", out);
            break;
        case EXPRESSION:
            gen_node(constants, size, function->first_arg, out);
            char* fop;
            switch(function->operator_type) {
                case SIN:
                    fop = "    fsin\n";
                    break;
                case COS:
                    fop = "    fcos\n";
                    break;
                case TAN:
                    fop = "    fptan\n    fstp st0\n";
                    break;
                case CTG:
                    fop = "    fptan\n    fdivp\n";
                    break;
                case PLUS:
                    fop = "    faddp\n";
                    gen_node(constants, size, function->second_arg, out);
                    break;
                case MINUS:
                    fop = "    fsubp\n";
                    gen_node(constants, size, function->second_arg, out);
                    break;
                case MULTIPLY:
                    fop = "    fmulp\n";
                    gen_node(constants, size, function->second_arg, out);
                    break;
                case DIVIDE:
                    fop = "    fdivp\n";
                    gen_node(constants, size, function->second_arg, out);
                    break;
            }
            fputs(fop, out);
            break;
    }
}

void gen_function(double* constants, size_t size, char* prefix, size_t number, AST* function, FILE* out) {
    fprintf(out, "%s%d:\n", prefix, number);
    gen_prologue(out);
    gen_node(constants, size, function, out);
    gen_epilogue(out);
    fputs("\n", out);
}

void gen_text(double* constants, size_t size, AST* functions[], AST* derivatives[], FILE* out) {
    fputs("section .text\n", out);
    for(size_t i = 0; i < 3; ++i) {
        gen_function(constants, size, "f", i + 1, functions[i], out);
        gen_function(constants, size, "df", i + 1, derivatives[i], out);
    }
    free(constants);
}

void gen_listing(double a, double b, AST* functions[], AST* derivatives[], FILE* out) {
    gen_header(out);
    double* constants;
    size_t size;
    gen_rodata(&constants, &size, a, b, functions, derivatives, out);
    gen_text(constants, size, functions, derivatives, out);
    for(int i = 0; i < 3; ++i) {
        destroy_tree(functions[i]);
        destroy_tree(derivatives[i]);
    }
}
