#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "equation_solving.h"
#include "cmp.h"

typedef enum EquationTypes{ 
    CONTRADICTION,  // a=0, b=0, c!=0 (противоречие)
    IDENTITY,       // a=0, b=0, c=0  (тождество)
    LINEAR,         // a=0, b!=0      (линейное)
    QUADRATIC       // a!=0           (квадратное)
} EquationTypes;

static EquationTypes GetEquationType(double a, double b, double c){
    if (IsZero(a))
    {
        if (IsZero(b))
        {
            return (IsZero(c)) ? IDENTITY : CONTRADICTION;
        }
        else /* if (b != 0) */
        {
            return LINEAR;
        }
    }
    else /* if (a != 0) */
    {
        return QUADRATIC;
    }
}

static void SolveLinear(double b, double c, double* x1){
    assert(x1);
    *x1 = -c / b;
}

static int SolveSquare(double a, double b, double c, double* d, double* x1, double* x2){
    assert(x1);
    assert(x2);

    *d = b*b - 4*a*c;
    if (IsZero(*d))
    {
        *x1 = *x2 = -b / (2*a);
        return ONE_ROOT;
    }
    else if (LessThan(*d, 0)) {
        return NO_ROOTS;
    }
    else
    {
        double sqrt_d = sqrt(*d);
        *x1 = (-b - sqrt_d) / (2*a);
        *x2 = (-b + sqrt_d) / (2*a);
        return TWO_ROOTS;
    }
}

int SolveEquation(double a, double b, double c, double* d, double* x1, double* x2) {
    assert(x1);
    assert(x2);

    EquationTypes equationType = GetEquationType(a, b, c);
    switch (equationType) {
        case LINEAR:
            SolveLinear(b, c, x1);
            return ONE_ROOT;
        case QUADRATIC:
            return SolveSquare(a, b, c, d, x1, x2);
        case CONTRADICTION:
            return NO_ROOTS;
        case IDENTITY:
            return INF_ROOTS;
        default:
            return SOLVE_ERROR;
    }

    return SOLVE_ERROR;
}
