#include "maths.h"

#include <stdio.h>

#define ACCURACY 0.001

extern double a, b;

double f1(double);
double f2(double);
double f3(double);
double df1(double);
double df2(double);
double df3(double);

function funcs[3] = {&f1, &f2, &f3};
function derivs[3] = {&df1, &df2, &df3};

int main(int argc, char** argv) {
    printf("%lf\n", area(a, b, funcs, derivs, ACCURACY));
    return 0;
}
