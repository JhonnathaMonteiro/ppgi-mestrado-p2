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

    //criacao da arvore de busca
    std::list<Node> arvore;

    //resolvendo o no raiz
    Node raiz;
    calcularSolucao(raiz, data);

    arvore.push_back(raiz);

    //branch and bound
    Node solucao = bnbComb(arvore, data);

    //tour size
    std::cout << "Solucao: " << solucao.lower_bound << std::endl;

    return 0;
}