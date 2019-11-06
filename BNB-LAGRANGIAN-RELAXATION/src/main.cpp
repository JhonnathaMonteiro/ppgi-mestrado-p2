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
    int dim = data->getDimension();

    //convertendo para uma matriz (double **)
    double **cost = new double *[dim];
    for (int i = 0; i < dim; i++)
    {
        cost[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            cost[i][j] = data->getDistance(i, j);
        }
    }

    // TODO: Gerar upper_bound com GILS-RVND-TSP
    // double UB = 426; //eil51.tsp <---- OTIMO
    double UB = 430; //eil51.tsp
    // double UB = 500; //eil51.tsp

    // double UB = 3323; //burma14.tsp
    // double UB = 3500; //burma14.tsp

    // double UB = 29368; //kroA200.tsp
    // double UB = 29368; //kroA200.tsp

    // double UB = 118282; //bier127.tsp
    // double UB = 119000; //bier127.tsp

    //branch and bound
    // int busca = BUSCA_BEST_BOUND;
    int busca = BUSCA_EM_LARGURA;
    // int busca = BUSCA_EM_PROFUNDIDADE;
    Node solucao = bnbComb(cost, dim, UB, busca);

    //free memory
    for (int i = 0; i < dim; ++i)
    {
        delete[] cost[i];
    }

    //solucao
    std::cout << "Solucao: " << solucao.LB << std::endl;

    return 0;
}