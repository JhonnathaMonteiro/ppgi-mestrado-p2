#include <iostream>
#include <vector>
#include <list>
#include <numeric>
#include <algorithm>

#include "data.h"
#include "hungarian.h"
#include "bnb_cnb.h"

int main(int argc, char **argv)
{
    //leitura da instancia
    Data *data = new Data(argc, argv[1]);
    data->readData();

    double **cost = new double *[data->getDimension()];
    for (int i = 0; i < data->getDimension(); i++)
    {
        cost[i] = new double[data->getDimension()];
        for (int j = 0; j < data->getDimension(); j++)
        {
            cost[i][j] = data->getDistance(i, j);
        }
    }

    //criacao da arvore de busca
    std::list<Node> arvore;

    //resolvendo o no raiz
    Node raiz;
    calcularSolucao(raiz, cost, data->getDimension());

    arvore.push_back(raiz);

    //branch and bound
    Node solucao = bnbComb(arvore, data, data->getDimension());

    //tour size
    std::cout << "Solucao: " << solucao.lower_bound << std::endl;

    return 0;
}