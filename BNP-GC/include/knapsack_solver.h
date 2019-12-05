#ifndef KNAPSACK_SOLVER_H_INCLUDED
#define KNAPSACK_SOLVER_H_INCLUDED

#include "matrix.h"

typedef struct
{
  int weight;
  double value;
} item_t;

Matrix<double> kp_solver(Matrix<double> &W,
                         Matrix<double> &PI, int c);

#endif // KNAPSACK_SOLVER_H_INCLUDED