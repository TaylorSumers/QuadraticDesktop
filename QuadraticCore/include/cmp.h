#ifndef CMP_H
#define CMP_H

#ifdef __cplusplus
extern "C" {
#endif  

#include <math.h>
#include <stdbool.h>

#define EPS 1e-6

bool EqualTo(double n1, double n2);

bool IsZero(double x);

bool GreaterThan(double n1, double n2);

bool LessThan(double n1, double n2);

#ifdef __cplusplus
}
#endif

#endif