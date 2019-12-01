#include <iostream>

#include "data_reader.h"
#include "bnp_gc.h"
#include "matrix.h"

int main(int argc, char **argv)
{

    // Leitura da instancia para o bnp
    DataReader<int> *data = new DataReader<int>(argc, argv[1]);
    std::vector<int> W = data->getW();
    // int c = data->getCap();

    Matrix<double> M(4, "EYE");
    M(0, 0) = 5;
    M(0, 1) = -2;
    M(0, 2) = 2;
    M(0, 3) = 7;

    M[1][1] = 1;
    M[1][1] = 0;
    M[1][2] = 0;
    M[1][3] = 3;

    M(2, 2) = -3;
    M(2, 2) = 1;
    M(2, 2) = 5;
    M(2, 3) = 0;

    M(3, 3) = 3;
    M(3, 3) = -1;
    M(3, 3) = -9;
    M(3, 3) = 4;

    // Matrix<double> M_adj = M.adjoint();

    for (unsigned i = 0; i < M.get_rows(); i++)
    {
        for (unsigned j = 0; j < M.get_cols(); j++)
        {
            std::cout << " " << M[i][j];
        }
        std::cout << std::endl;
    }

    // std::cout << "DETERMINANT: " << M.determinant(M, M.get_rows()) << std::endl;

    // for (auto &i : W)
    // {
    //     std::cout << i << std::endl;
    // }

    return 0;
}
