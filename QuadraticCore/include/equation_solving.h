#ifndef EQUATION_SOLVING_H
#define EQUATION_SOLVING_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RootAmount{
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS,
    INF_ROOTS,
    SOLVE_ERROR
} RootAmount;

int SolveEquation(double a, double b, double c, double* d, double* x1, double* x2);

#ifdef __cplusplus
}
#endif

#endif