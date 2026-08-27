#include "pch.h"
#include "QuadraticCoreAdapter.h"
#include "equation_solving.h"

namespace quadratic::interop {
  Result Solve(double a, double b, double c) {
    Result result{};
    result.nRoots = SolveEquation(a, b, c, &result.d, &result.x1, &result.x2);
    return result;
  }
}