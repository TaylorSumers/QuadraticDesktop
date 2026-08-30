#pragma once

#include "graph_analysis.h"
#include "equation_solving.h"

namespace winrt::QuadraticDesktop::Interop {
  struct GraphInfo {
    Point vertex{};
    BranchesDirection branchesDirection{};
  };

  struct Solution {
    RootAmount nRoots{};
    double d{};
    double x1{};
    double x2{};
  };

  Solution Solve(double a, double b, double c);
  GraphInfo GetGraphInfo(double a, double b, double c);
}