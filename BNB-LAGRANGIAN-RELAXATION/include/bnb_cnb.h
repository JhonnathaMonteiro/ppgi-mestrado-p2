#ifndef BNB_CNB_H_INCLUDED
#define BNB_CNB_H_INCLUDED

//Metodos de busca
#define BUSCA_EM_PROFUNDIDADE 0
#define BUSCA_EM_LARGURA 1
#define BUSCA_BEST_BOUND 2

#include <vector>
#include <list>
#include <utility>
#include <numeric>
#include "data.h"

// Representacao da arvore de busca
typedef struct node
{
    std::vector<std::pair<int, int>> arcos_proibidos; // lista de arcos proibidos
    std::vector<double> multiplicadores;              // multiplicadores de lagrange do no
                                                      // solucao do algoritmo hungaro)
    std::vector<std::pair<int, int>> one_tree;        // 1-arvore solucao modificada do kruskal
    int maior_grau_i;                                 // indice do vertice com maior grau
    double LB;                                        // valor da solucao
    bool otimo = false;                               // variavel que diz se o no deve gerar filhos
    bool is_upper_bound = false;                      // true: solucao otima = UB
} Node;

Node bnbComb(double **, int, double, int);

#endif // BNB_CNB_H_INCLUDED