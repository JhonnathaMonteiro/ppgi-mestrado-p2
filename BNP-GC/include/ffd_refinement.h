#ifndef FFD_REF_H_INCLUDED
#define FFD_REF_H_INCLUDED

#include "matrix.h"

void ffd_refinament(Matrix<double> &,
                    Matrix<double> &,
                    Matrix<double> &, double);

#endif // FFD_REF_H_INCLUDED