#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "announ.h"

#define ABS(x) ((x) > 0) ? (x) : (-(x))

typedef float(*ptrFunc) (float);


ptrFunc functions[3] = {&f1, &f2, &f3};
ptrFunc d_functions[3] = {&df1, &df2, &df3};

static struct {
    float EXP;
    float A_exam;
    float B_exam;
    char FLAG_ABSC : 1;
    char FLAG_ITER : 1;
    char FLAG_SQ : 1;
    char FUNC_ROOT_1 : 2;
    char FUNC_ROOT_2 : 2;
    char FUNC_INTEGRAL: 2;
} FLAGS = {0.001, 0, 0, 0, 0, 0, -1, -1, -1};

// *_______________________________________________________________________________*



// FIND THE ROOT F1 - F2 = 0:

float rootNewton(ptrFunc f, ptrFunc df, ptrFunc d2f,
                ptrFunc g,  ptrFunc dg, ptrFunc d2g,
                    float a, float b, float eps1) {
    float x0,xn;// вычисляемые приближения для корня
    int i=0;
    if ((f(a)-g(a))*(f(b)-g(b))>0) // если знаки функции на краях отрезка одинаковые, то здесь нет корня
        return -1;
    else{
        if ((f(a)-g(a))*(d2f(a)-d2g(a))>0)
            x0  = a; // для выбора начальной точки проверяем f(x0)*d2f(x0)>0 ?
        else
            x0 = b;
        xn = x0-(f(x0)-g(x0))/(df(x0)-dg(x0)); // считаем первое приближение
        while(fabs(x0-xn) > eps1){
              x0 = xn;
              xn = x0-(f(x0)-g(x0))/(df(x0)-dg(x0)) ; // формула Ньютона
              ++i;
        }
        if(FLAGS.FLAG_ITER) printf("AMOUNT OF ITERATIONS: %d\n", i);
        return xn;
    }

    return 0;
}

// INTEGRAL:


float trapezoidalIntegral(ptrFunc f, float a, float b, float eps2){
    float width;
    float sq1 = 0, sq2 = 0, n = 1, trapezoidal_integral = 0;

    f = *f;

    while (ABS(sq2 - sq1) > eps2); {
        width = (b-a)/n;
        sq1 = sq2;

        for(int step = 0; step < n; step++) {
            const double x1 = a + step*width;
            const double x2 = a + (step+1)*width;

            trapezoidal_integral += 0.5*(x2-x1)*(f(x1) + f(x2));
        }

        n *= 2;
    }
    

    return trapezoidal_integral;
}

// *_______________________________________________________________________________*

int main (int argc, char *argv[]) {
    // command-line arguments
    int rez = 0;
    while ( (rez = getopt(argc, argv, "hanSR:I:E:")) != -1){
        switch (rez) {
            case 'h': printf("    	  ВЫЧИСЛЕНИЕ МНОГОМЕРНЫХ КОРНЕЙ И ИНТЕГРАЛЬНЫХ УРАВНЕНИЙ         \n"
                             "==========================HELP==========================\n"
                             "-a \t вывести абсциссы точек пересечения функций\n"
                             "-n \t вывести количество итераций\n"
                             "-S \t вывести искомую площадь\n"
                             "-R <func1:(1,2,3)> <func2:(1,2,3)>\n"
                             " \t поиск корня для функций: <func1 : (1,2,3)> и <func2:(1,2,3)>\n"
                             "-I <func:(1,2,3)> <a> <b>\n"
                             " \t показать интеграл <func:(1,2,3)> от a до b\n"
                             "-E <fnum> изменение точности вычисления(default: 0.001)\n"
                             "========================================================\n"); break;
            case 'a': FLAGS.FLAG_ABSC = 1; break;
            case 'n': FLAGS.FLAG_ITER = 1; break;
            case 'S': FLAGS.FLAG_SQ = 1; break;
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

    if(FLAGS.FUNC_INTEGRAL == 0){
        printf("Интеграл от F1 = 3*(0.5 / (x + 1) + 1) в промежутках: %f до %f ", FLAGS.A_exam, FLAGS.B_exam);
        printf("равен: %f;\n", trapezoidalIntegral(f1, FLAGS.A_exam, FLAGS.B_exam, FLAGS.EXP));
    }
    else if(FLAGS.FUNC_INTEGRAL == 1){
        printf("Интеграл от F2 = 2.5*x − 9.5 в промежутках: %f до %f ", FLAGS.A_exam, FLAGS.B_exam);
        printf("равен: %f;\n", trapezoidalIntegral(f2, FLAGS.A_exam, FLAGS.B_exam, FLAGS.EXP));
    }
    else if(FLAGS.FUNC_INTEGRAL == 2){
        printf("Интеграл от F3 = 5 / x (x > 0) в промежутках: %f до %f ", FLAGS.A_exam, FLAGS.B_exam);
        printf("равен: %f;\n", trapezoidalIntegral(f3, FLAGS.A_exam, FLAGS.B_exam, FLAGS.EXP));
    }


    float a, b, c, s, s1, s2, s3; // a = f1 and f2; b = f2 and f3; c = f1 and f3
        //s - square DEADinside; s1 - under f1; s2 - under f2; s3 - under f3
    c = rootNewton(f1, df1, d2f1, f2, df2, d2f2, 5.0, 5.25, FLAGS.EXP);
    b = rootNewton(f2, df2, d2f2, f3, df3, d2f3, 4.0, 4.5, FLAGS.EXP);
    a = rootNewton(f1, df1, d2f1, f3, df3, d2f3, 1.0, 1.5, FLAGS.EXP);

    if(FLAGS.FUNC_ROOT_1 == 0){
        if(FLAGS.FUNC_ROOT_2 == 1)
            printf("Точка пересечения функций: %f", rootNewton(f1, df1, d2f1, f2, df2, d2f2, 5.0, 5.25, FLAGS.EXP));
        else
            printf("Точка пересечения функций: %f", rootNewton(f1, df1, d2f1, f3, df3, d2f3, 1.0, 1.5, FLAGS.EXP));
    }
    else if(FLAGS.FUNC_ROOT_1 == 1){
        if(FLAGS.FUNC_ROOT_2 == 0)
            printf("Точка пересечения функций: %f", rootNewton(f1, df1, d2f1, f2, df2, d2f2, 5.0, 5.25, FLAGS.EXP));
        else
            printf("Точка пересечения функций: %f", rootNewton(f2, df2, d2f2, f3, df3, d2f3, 4.0, 4.5, FLAGS.EXP));
    }
    else if(FLAGS.FUNC_ROOT_1 == 2){
        if(FLAGS.FUNC_ROOT_2 == 0)
            printf("Точка пересечения функций: %f", a = rootNewton(f1, df1, d2f1, f3, df3, d2f3, 1.0, 1.5, FLAGS.EXP));
        else
            printf("Точка пересечения функций: %f", rootNewton(f2, df2, d2f2, f3, df3, d2f3, 4.0, 4.5, FLAGS.EXP));
    }

    s1 = trapezoidalIntegral(f1, a, c, FLAGS.EXP);
    s2 = trapezoidalIntegral(f2, b, c, FLAGS.EXP);
    s3 = trapezoidalIntegral(f3, a, b, FLAGS.EXP);
    s = s1 - s2 - s3;
    if (FLAGS.FLAG_SQ) printf("%f", s);

    return 0;
}