#include "pch.h"
#include "QuadraticCoreAdapter.h"

namespace winrt::QuadraticDesktop::Interop {
  Solution Solve(double a, double b, double c) {
    Solution result{};
    result.nRoots = SolveEquation(a, b, c, &result.d, &result.x1, &result.x2);
    return result;
  }

  GraphInfo GetGraphInfo(double a, double b, double c) {
    GraphInfo graphInfo{
      .vertex = GetVertex(a, b, c),
      .branchesDirection = GetBranchesDirection(a)
    };
    return graphInfo;
  }
}