#include "cmp.h"
#include "graph_analysis.h"

#ifdef __cplusplus
extern "C" {
#endif

static double NormalizeZero(double value) {
  return value == 0.0 ? 0.0 : value;
}

BranchesDirection GetBranchesDirection(double a) {
  if (GreaterThan(a, 0)) {
    return UP;
  }
  else if (LessThan(a, 0)) {
    return DOWN;
  }
  else {
    return NO_BRANCHES;
  }
}

Point GetVertex(double a, double b, double c) {
  Point vertex = { .x = NAN, .y = NAN };
  if (EqualTo(a, 0)) {
    return vertex;
  }

  vertex.x = NormalizeZero(- b / (2 * a));
  vertex.y = NormalizeZero(a * (vertex.x * vertex.x) + b * vertex.x + c);
  return vertex;
}

#ifdef __cplusplus
}
#endif