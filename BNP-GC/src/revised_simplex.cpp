#include "revised_simplex.h"
#include "matrix.h"

RV rv_simplex(Matrix<double> W,
              Matrix<double> b,
              Matrix<double> C,
              Matrix<double> B,
              double c, double tol_gc, unsigned n)
{

  Matrix<double> B_inv = B.inverse();

  while (true)
  {
    Matrix<double> X_B = B_inv * b;
    Matrix<double> PI = C.transpose() * B_inv;
    Matrix<double> OPT = PI * b;
    break;
  }
}