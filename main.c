#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "funcfromasm.h"

#define ABS(x) ((x) > 0) ? (x) : (-(x))

#define RF12X 1
#define RF12Y 2

#define RF13X -2
#define RF13Y -1

#define RF23X 0.1
#define RF23Y 1

typedef float(*ptrFunc) (float);


ptrFunc functions[3] = {&f1, &f2, &f3};
ptrFunc d_functions[3] = {&df1, &df2, &df3};
ptrFunc d2_functions[3] = {&d2f1, &d2f2, &d2f3};

static struct {
    float EXP;
    float A_exam;
    float B_exam;
    char FLAG_ABSC : 1;
    char FLAG_ITER : 1;
    char FUNC_ROOT_1 : 2;
    char FUNC_ROOT_2 : 2;
    char FUNC_INTEGRAL: 2;
} FLAGS = {0.001, 0, 0, 0, 0, -1, -1, -1};

// FIND THE ROOT F1 x F2 (Combined method)

float root(ptrFunc f, ptrFunc df, ptrFunc d2f,
            ptrFunc g,  ptrFunc dg, ptrFunc d2g,
            float a, float b, float eps1) {

	int iter_num = 0;

    f = *f; df = *df; d2f = *d2f;
    g = *g; dg = *dg; d2g = *d2g;

	while (ABS(a - b) > 2*eps1) {

	    if (((f(a) - g(a)) * (d2f(a) - d2g(a))) < 0) {
	        a = a - ((f(a) - g(a)) * (a - b))/(f(a) - f(b));
	    }
	    else {
	        a = a - (f(a) - g(a))/(df(a)-dg(a));
	    }
	    if (((f(b) - g(b)) * (d2f(b) - d2g(b))) < 0) {
	        b = b - ((f(b) - g(b)) * (b - a))/(f(b) - f(a));
	    }
	    else {
	        b = b - (f(b) - g(b))/(df(b)-dg(b));
	    }
	   	++iter_num;

	}
	if (FLAGS.FLAG_ITER) printf("NUMBER OF ITERATIONS = %d.\n", iter_num);
    return (a+b)/2;
}
// INTEGRAL (SIMPSON FORMULA)

float integral(ptrFunc f, float a, float b, float eps2) {
    int N = 1;
    static float h, sq1, sq2;

    f = *f;

    do {
        h = (b-a) / (float)N;
        sq1 = sq2;

        float k1 = 0, k2 = 0;
        for(int i = 1; i < N; i += 2) {
            k1 += f(a + (float)i * h);
            k2 += f(a + (float)(i+1) * h);
        }
        sq2 =  h/3 * (f(a) + 4 * k1 + 2 * k2);
        N *= 2;
    }
    while (ABS(sq2 - sq1) > eps2);

    return sq2;
}

// *_______________________________________________________________________________*

int main (int argc, char *argv[]) {
    // command-line arguments
    int rez = 0;
    while ( (rez = getopt(argc, argv, "hanR:I:E:")) != -1){
        switch (rez) {
            case 'h': printf("    	  FINDING THE ROOTS and THE AREA OF FUNC`s        \n"
                             "        f1 = 1 + 4/(x^2 + 1) f2 = x^3 f3 = 2^(-x)       \n"
                             "==========================HELP==========================\n"
                             "-a \t show the abscissa intersections of functions\n"
                             "-n \t show the number of iterations\n"
                             "-R <func1:(1,2,3)> <func2:(1,2,3)>\n"
                             " \t func root for <func1 : (1,2,3)> and <func2:(1,2,3)>\n"
                             "-I <func:(1,2,3)> <a> <b>\n"
                             " \t func integral for <func:(1,2,3)> from a to b\n"
                             "-E <fnum> changing the eps of algorithms(default: 0.001)\n"
                             "========================================================\n"); break;
            case 'a': FLAGS.FLAG_ABSC = 1; break;
            case 'n': FLAGS.FLAG_ITER = 1; break;
            case 'R': {
                FLAGS.FUNC_ROOT_1 = atoi(argv[optind-1]) - 1;
                FLAGS.FUNC_ROOT_2 = atoi(argv[optind]) - 1;
                break;
            }
            case 'I': {
                FLAGS.FUNC_INTEGRAL= atoi(argv[optind-1]) - 1;
                FLAGS.A_exam = (float)atof(argv[optind]);
                FLAGS.B_exam = (float)atof(argv[++optind]);
                break;
            }
            case 'E': FLAGS.EXP = (float)atof(optarg); break;
        } // switch
    } // while

    float exp1 = FLAGS.EXP / 40.0;
    float exp2 = FLAGS.EXP / 2.0;

    float c = root(&f1, &df1, &d2f1, &f2, &df2, &d2f2, RF12X, RF12Y, exp1);
    float a = root(&f1, &df1, &d2f1, &f3, &df3, &d2f3, RF13X, RF13Y, exp1);  
    float b = root(&f2, &df2, &d2f2, &f3, &df3, &d2f3, RF23X, RF23Y, exp1);

    // a < b < c - пределы интегрирования

    if ((FLAGS.FUNC_ROOT_1 != -1) && (FLAGS.FUNC_ROOT_2 != -1)) {
        float x = root(functions[FLAGS.FUNC_ROOT_1], d_functions[FLAGS.FUNC_ROOT_1], d2_functions[FLAGS.FUNC_ROOT_1],
                       functions[FLAGS.FUNC_ROOT_2], d_functions[FLAGS.FUNC_ROOT_2], d2_functions[FLAGS.FUNC_ROOT_2],
                       -2, 2, exp1);
        printf("Functions intersect in: x = %f, y = %f.\n", x, (*functions[FLAGS.FUNC_ROOT_1])(x));
    }

    if (FLAGS.FUNC_INTEGRAL != -1) {
        printf("Integral of function: %f.\n", integral(functions[FLAGS.FUNC_INTEGRAL], FLAGS.A_exam, FLAGS.B_exam, exp2));
    }

    if (FLAGS.FLAG_ABSC) {
        printf("f1 x f2 : x = %f, y= %f\n", c, f1(c));
        printf("f1 x f3 : x = %f, y= %f\n", a, f1(a));
        printf("f2 x f3 : x = %f, y= %f\n", b, f2(b));
    }

    static float area;

    area += integral(&f1, a, c, exp2);
    area -= integral(&f3, a, b, exp2);
    area -= integral(&f2, b, c, exp2);

    printf("The area of figure given by the func`s: %f. \n", area);

    return 0;

}