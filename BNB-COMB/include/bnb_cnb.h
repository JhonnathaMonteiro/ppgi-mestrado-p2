#ifndef BNB_CNB_H_INCLUDED
#define BNB_CNB_H_INCLUDED
#include <vector>
#include <list>
#include <utility>
#include <numeric>
#include "data.h"

// Representacao da arvore de busca
typedef struct node
{
    std::vector<std::pair<int, int>> arcos_proibidos; // lista de arcos proibidos
    // std::vector<std::pair<int, int>> arcos_obrigatorios; // lista de arcos obrigatorios
    std::vector<std::vector<int>> subtour; // conjunto de subtours da solucao
                                           // gerada pelo alg. hungaro
    double lower_bound;                    //lower_bound produzido pelo no (ou custo total da
                                           //solucao do algoritmo hungaro)
    int escolhido;                         // subtour escolhido dado o criterio de selecao
    bool podar = false;                    // variavel que diz se o no deve gerar filhos
} Node;

void calcularSolucao(Node &, double **, int);
Node bnbComb(std::list<Node>, Data *, int, double);

#endif // BNB_CNB_H_INCLUDED