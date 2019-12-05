#include <iostream>
#include <math.h>

#include "data_reader.h"
#include "bnp_gc.h"
#include "matrix.h"
#include "revised_simplex.h"
#include "ffd_refinement.h"

int main(int argc, char **argv)
{

    // Leitura da instancia para o bnp
    DataReader<double> *data = new DataReader<double>(argc, argv[1]);
    std::vector<double> _W = data->getW();
    Matrix<double> W(_W);         // weights of the items
    unsigned n = data->getN();    // number of items
    int c = data->getCap();       // bin capacity
    Matrix<double> b(n, 1, 1);    // right hand side of the constraints
    Matrix<double> C(n, 1, 1);    // patterns in objective function
    double tol_gc = pow(10, -10); // tolerancia

    // BFS
    // Aqui to utilizando a aproximacao singleton (um item em cada bin), sei que
    // uma aproximacao mais adequada seria utilizando um algoritmo de FFD
    // (First Fit Decreasing Algorithm) refinado, mas por simplicidade vou manter
    // singleton por hora.
    Matrix<double> B(n, "EYE");

    std::cout << "Instance: " << data->getID() << std::endl;

    RV soluc = rv_simplex(W, b, C, B, c, tol_gc, n);
    std::cout << "==========SOLUCAO==========" << std::endl;
    for (unsigned i = 0; i < n; i++)
    {
        /* code */
        std::cout << "X_B[" << i << "]: " << soluc.X_B[i][0] << std::endl;
    }

    std::cout << "==========MATRIZ==========" << std::endl;
    for (unsigned i = 0; i < n; i++)
    {
        for (unsigned j = 0; j < n; j++)
        {
            std::cout << " " << soluc.B[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "==========BINS==========" << std::endl;
    std::cout << "OTIMO (BINS): " << soluc.OPTCG << std::endl;

    return 0;
}
