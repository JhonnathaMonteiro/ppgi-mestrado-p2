#ifndef RV_SIMPLEX_H_INCLUDED
#define RV_SIMPLEX_H_INCLUDED

#include "matrix.h"

typedef struct rv_solution
{
  Matrix<double> B;   //basis of revised simplex method
  Matrix<double> X_B; //values of the basic solutions
  double OPTCG;       //optimal value of the problem
} RV;

RV rv_simplex(Matrix<double>,
              Matrix<double>,
              Matrix<double>,
              Matrix<double>,
              double, double, unsigned);

#endif // RV_SIMPLEX_H_INCLUDED