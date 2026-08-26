#include "cmp.h"

#ifdef __cplusplus
extern "C" {
#endif


bool EqualTo(double n1, double n2) {
    return fabs(n1 - n2) <= EPS;
}

bool IsZero(double x) {
    return EqualTo(x, 0);
}

bool GreaterThan(double n1, double n2){
    return n1 - n2 > EPS;
}

bool LessThan(double n1, double n2){
    return n1 - n2 < -EPS;
}

#ifdef __cplusplus
}
#endif