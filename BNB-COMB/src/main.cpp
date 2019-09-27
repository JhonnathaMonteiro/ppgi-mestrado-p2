#include <iostream>
#include <vector>
#include <utility>
using namespace std;

#include "data.h"
#include "hungarian.h"

// Representacao da arvore de busca
typedef struct node
{
    std::vector<pair<int, int>> arcos_proibidos; // lista de arcos proibidos
    std::vector<std::vector<int>> subtour;       // conjunto de subtours da solucao
                                                 // gerada pelo alg. hungaro
    double lower_bound;                          //lower_bound produzido pelo no (ou custo total da
                                                 //solucao do algoritmo hungaro)
    int escolhido;                               // subtour escolhido dado o criterio de selecao
    bool podar;                                  // variavel que diz se o no deve gerar filhos
} Node;

void calcularSolucao(Node, double **cost, Data *data)
{

    hungarian_problem_t p;
    int mode = HUNGARIAN_MODE_MINIMIZE_COST;
    hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode); // Carregando o problema

    double obj_value = hungarian_solve(&p);

    // liberando memoria
    hungarian_free(&p);
    for (int i = 0; i < data->getDimension(); i++)
        delete[] cost[i];
    delete[] cost;
    delete data;

    cout << "Obj. value: " << obj_value << endl;
}

int main(int argc, char **argv)
{

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

    // cout << "Assignment" << endl;
    // hungarian_print_assignment(&p);

    // criacao da arvore de busca
    // arvore = std::list<Node>;

    Node raiz;
    calcularSolucao(raiz, cost, data); // calcular solucao usando algoritmo hungaro e matriz
                                       // de distancia inicial e preencher os atributos da
                                       // estrutura  Node de acordo

    // arvore.push_back(raiz);

    // for (int i = 0; i < raiz.subtour[raiz.escolhido].size() - 1; i++)
    // {
    //     Node n;
    //     n.arcos_proibidos = raiz.arcos_proibidos;

    //     std::pair<int> arco_proibido;
    //     arco_proibido.first = i;
    //     arco_proibido.second = i + 1;

    //     n.arcos_proibidos.push_back(arco_proibido);
    //     n.calcularSolucao();
    //     arvore.pushback(n); //inserir novos nos na arvore
    // }

    return 0;
}