#ifndef GRAPH_ANALYSIS_H
#define GRAPH_ANALYSIS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BranchesDirection {
  UP,
  DOWN,
  NO_BRANCHES
} BranchesDirection;

typedef struct Point {
  double x;
  double y;
} Point;

BranchesDirection GetBranchesDirection(double a);
Point GetVertex(double a, double b, double c);

#ifdef __cplusplus
}
#endif

#endif