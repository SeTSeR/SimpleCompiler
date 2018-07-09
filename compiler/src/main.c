#include "ast.h"
#include "codegen.h"
#include "derivative.h"
#include "error.h"
#include "parser.h"
#include "optimizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_derivative() {
    char test1[] = "2 x +";
    char ans1[] = "0 1 +";
    char test2[] = "x sin x cos *";
    char ans2[] = "x cos 1 * x cos * x sin 0 x sin 1 * - * +";
    char test3[] = "x tan x ctg +";
    char ans3[] = "1 x cos x cos * / 0 1 x sin x sin * / - +";
    printf("%d %d %d",
            equals(derivative(parse(test1)), parse(ans1)),
            equals(derivative(parse(test2)), parse(ans2)),
            equals(derivative(parse(test3)), parse(ans3)));
}

int main(int argc, char** argv) {
    if(argc != 3) error("You must provide exactly two arguments: name of input file and name of output file.");
    FILE *infile = fopen(argv[1], "rt");
    char buf[256];
    fgets(buf, 256, infile);
    double a, b;
    if(sscanf(buf, "%lf %lf", &a, &b) < 2) error("You must provide two numbers at the first line.");
    AST* functions[3];
    AST* derivatives[3];
    for(size_t i = 0; i < 3; ++i) {
        fgets(buf, 256, infile);
        functions[i] = parse(buf);
    }
    fclose(infile);
    for(int i = 0; i < 3; ++i) derivatives[i] = derivative(functions[i]);
    for(int i = 0; i < 6; ++i) optimize(functions[i]);
    char *listing = gen_listing(a, b, functions, derivatives);
    FILE *outfile = fopen(argv[2], "wt");
    fclose(outfile);
    return 0;
}
