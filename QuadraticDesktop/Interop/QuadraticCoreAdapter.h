#pragma once

#include "equation_solving.h"
#include "graph_analysis.h"

namespace winrt::QuadraticDesktop::Interop {
  struct GraphInfo {
    Point vertex{};
    BranchesDirection branchesDirection{};
  };

  struct Solution {
    int nRoots{};
    double d{};
    double x1{};
    double x2{};
  };

  Solution Solve(double a, double b, double c);
  GraphInfo GetGraphInfo(double a, double b, double c);
}