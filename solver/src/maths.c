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

double integrate(function f, double a, double b, double eps) {
	int n = 1000;
	double sum1 = 0, sum2 = 0;
	double step = (b - a) / n;
	for(int i = 0; i < n; ++i) {
		sum2 += ((*f)(a + i * step) + 4*(*f)(a + (i + 0.5) * step) + (*f)(a + (i + 1) * step))*step/6;
	}
	while(fabs(sum2 - sum1) >= eps) {
		sum1 = sum2;
		sum2 = 0;
		n *= 2;
		step = (b - a) / n;
		for(int i = 0; i < n; ++i) {
			sum2 += ((*f)(a + i * step) + 4*(*f)(a + (i + 0.5) * step) + (*f)(a + (i + 1) * step))*step/6;
		}
	}
	return sum2;
}

static double solve(function f, function df, double a, double b, double eps) {
	while((b - a) > (2 * eps)) {
		double sign2 = (*df)(a + eps) - (*df)(a);
		if((*f)(a) * sign2 < 0)
			a = a - ((*f)(a))*(a - b)/((*f)(a) - (*f)(b));
		else
			a = a - (*f)(a)/((*df)(a));
		sign2 = (*df)(b + eps) - (*df)(b);
		if((*f)(b) * sign2 < 0)
			b = b - ((*f)(b))*(b - a)/((*f)(b) - (*f)(a));
		else
			b = b - (*f)(b)/((*df)(b));
	}
	return a + (b - a) / 2;
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

    givenf = functions[2];
    givendf = derivatives[2];
    giveng = functions[0];
    givendg = derivatives[0];
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

        givenf = functions[2];
        givendf = derivatives[2];
        giveng = functions[0];
        givendg = derivatives[0];
        double x3 = solve(&fming, &dfmindg, a, b, eps1);
	}

    return fabs(integrate(functions[0], x1, x3, eps2)
            + integrate(functions[2], x3, x2, eps2)
            + integrate(functions[1], x2, x1, eps2));
}
