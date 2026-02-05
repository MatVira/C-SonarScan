
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "tinyexpr.h"



#define loops 10000



typedef double (*function1)(double);

void bench(const char *expr, function1 func) {
    int i, j;
    double d;
    double tmp;
    clock_t start;

    te_variable lk = {"a", &tmp};

    printf("Expression: %s\n", expr);

    printf("native ");
    start = clock();
    d = 0;
    for (j = 0; j < loops; ++j)
        for (i = 0; i < loops; ++i) {
            tmp = i;
            d += func(tmp);
        }
    const int nelapsed = (clock() - start) * 1000 / CLOCKS_PER_SEC;

    /*Million floats per second input.*/
    printf(" %.5g", d);
    if (nelapsed)
        printf("\t%5dms\t%5dmfps\n", nelapsed, loops * loops / nelapsed / 1000);
    else
        printf("\tinf\n");




    printf("interp ");
    te_expr *n = te_compile(expr, &lk, 1, 0);
    start = clock();
    d = 0;
    for (j = 0; j < loops; ++j)
        for (i = 0; i < loops; ++i) {
            tmp = i;
            d += te_eval(n);
        }
    const int eelapsed = (clock() - start) * 1000 / CLOCKS_PER_SEC;
    te_free(n);

    /*Million floats per second input.*/
    printf(" %.5g", d);
    if (eelapsed)
        printf("\t%5dms\t%5dmfps\n", eelapsed, loops * loops / eelapsed / 1000);
    else
        printf("\tinf\n");


    printf("%.2f%% longer\n", (((double)eelapsed / nelapsed) - 1.0) * 100.0);


    printf("\n");
}


double a5(double a) {
    return a+5;
}

double a55(double a) {
    return 5+a+5;
}

double a5abs(double a) {
    return fabs(a+5);
}

double a52(double a) {
    return (a+5)*2;
}

double a10(double a) {
    return a+(5*2);
}

double as(double a) {
    return sqrt(pow(a, 1.5) + pow(a, 2.5));
}

double al(double a) {
    return (1/(a+1)+2/(a+2)+3/(a+3));
}

int main(int argc, char *argv[])
{

    bench("a+5", a5);
    bench("5+a+5", a55);
    bench("abs(a+5)", a5abs);

    bench("sqrt(a^1.5+a^2.5)", as);
    bench("a+(5*2)", a10);
    bench("(a+5)*2", a52);
    bench("(1/(a+1)+2/(a+2)+3/(a+3))", al);

    return 0;
}
