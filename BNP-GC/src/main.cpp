#include <iostream>
#include <math.h>

#include "data_reader.h"
#include "bnp_gc.h"
#include "matrix.h"
#include "revised_simplex.h"

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

    // Matrix<double> B(10, "EYE");
    // Matrix<double> B2 = B.inverse();

    // for (unsigned i = 0; i < 10; i++)
    // {
    //     for (unsigned j = 0; j < 10; j++)
    //     {
    //         std::cout << " " << B2[i][j];
    //     }
    //     std::cout << std::endl;
    // }

    RV soluc = rv_simplex(W, b, C, B, c, tol_gc, n);
    std::cout << "==========SOLUCAO==========" << std::endl;
    std::cout << "OTIMO: " << data->getOpt() << std::endl;
    for (unsigned i = 0; i < n; i++)
    {
        /* code */
        std::cout << "X_B: " << soluc.X_B[i][0] << std::endl;
    }

    return 0;
}
