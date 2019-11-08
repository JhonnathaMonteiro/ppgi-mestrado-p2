#include <iostream>
#include <vector>
#include <list>
#include <numeric>
#include <algorithm>
#include <data.h>

#include "bnb_cnb.h"
#include "sub_gradient.h"

int main(int argc, char **argv)
{
    //leitura da instancia para o bnb
    Data *data = new Data(argc, argv[1]);
    data->readData();
    size_t dim = data->getDimension();

    //convertendo para uma matriz (double **)
    double **cost = new double *[dim];
    for (size_t i = 0; i < dim; i++)
    {
        cost[i] = new double[dim];
        for (size_t j = 0; j < dim; j++)
        {
            cost[i][j] = data->getDistance(i, j);
        }
    }

    // TODO: Gerar upper_bound com GILS-RVND-TSP
    // double UB = 426; //eil51.tsp <---- OTIMO
    // double UB = 427; //eil51.tsp < nao viavel
    // double UB = 500; //eil51.tsp

    double UB = 3323; //burma14.tsp < otimo
    // double UB = 3324; //burma14.tsp < n viavel
    // double UB = 3500; //burma14.tsp < n viavel
    // double UB = 4164; //burma14.tsp < viavel

    //branch and bound
    // int busca = BUSCA_BEST_BOUND;
    int busca = BUSCA_EM_LARGURA;
    // int busca = BUSCA_EM_PROFUNDIDADE;

    Node solucao = bnbComb(cost, dim, UB, busca);

    //free memory
    for (size_t i = 0; i < dim; ++i)
    {
        delete[] cost[i];
    }

    //solucao
    std::cout << "Solucao: " << solucao.LB << std::endl;

    return 0;
}