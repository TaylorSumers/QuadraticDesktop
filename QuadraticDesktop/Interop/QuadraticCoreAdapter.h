#pragma once

namespace quadratic::interop {
  struct Result {
    int nRoots{};
    double x1{};
    double x2{};
  };

  Result Solve(double a, double b, double c);
}