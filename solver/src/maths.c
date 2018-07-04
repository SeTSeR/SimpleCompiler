#include "maths.h"

#include <math.h> // for fabs

static function givenf, giveng, givendf, givendg;

inline static double fming(double x) {
    return (*givenf)(x) - (*giveng)(x);
}

inline static double dfmindg(double x) {
    return (*givendf)(x) - (*givendg)(x);
}

inline static double dist(double x, double y, double z) {
	return x * x + y * y + z * z;
}

double integrate(double (*f)(double), double a, double b, double eps) {
    return 0;
}

double solve(double (*f)(double), double (*df)(double), double a, double b, double eps) {
    return 0;
}

double area(double a, double b, function functions[3], function derivatives[3], double eps) {
    double eps2 = eps/sqrt(3); // accuracy of integrating
    double eps1 = eps/10; // this accuracy is defined randomly, it's need for initial values of roots,
    givenf = functions[0]; // which can be calculated with better accuracy later, if needed.
    givendf = derivatives[0];
    giveng = functions[1];
    givendg = derivatives[1];
    double x1 = solve(&fming, &dfmindg, a, b, eps1);

    givenf = functions[1];
    givendf = derivatives[1];
    giveng = functions[2];
    givendg = derivatives[2];
    double x2 = solve(&fming, &dfmindg, a, b, eps1);

    givenf = functions[3];
    givendf = derivatives[3];
    giveng = functions[1];
    givendg = derivatives[1];
    double x3 = solve(&fming, &dfmindg, a, b, eps1);

    double neweps1 = eps1 / dist(functions[1](x1) - functions[0](x1),
            functions[2](x2) - functions[1](x2),
            functions[0](x3) - functions[2](x3)); // New accuracy, roots may need to be recalculated

    if(neweps1 < eps1) {
		eps1 = neweps1;
        givenf = functions[0];
        givendf = derivatives[0];
        giveng = functions[1];
        givendg = derivatives[1];
        double x1 = solve(&fming, &dfmindg, a, b, eps1);

        givenf = functions[1];
        givendf = derivatives[1];
        giveng = functions[2];
        givendg = derivatives[2];
        double x2 = solve(&fming, &dfmindg, a, b, eps1);

        givenf = functions[3];
        givendf = derivatives[3];
        giveng = functions[1];
        givendg = derivatives[1];
        double x3 = solve(&fming, &dfmindg, a, b, eps1);
	}

    return fabs(integrate(functions[0], x1, x3, eps2) + integrate(functions[1], x3, x2, eps2) + integrate(functions[2], x2, x1, eps2));
}
