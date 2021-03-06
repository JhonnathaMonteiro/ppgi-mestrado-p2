#ifndef BNB_CNB_H_INCLUDED
#define BNB_CNB_H_INCLUDED

#include <vector>
#include <list>
#include <utility>
#include <numeric>
#include "data.h"

//Metodos de busca
#define BUSCA_EM_PROFUNDIDADE 0
#define BUSCA_EM_LARGURA 1
#define BUSCA_BEST_BOUND 2

// Constantes
#define EPSILON std::numeric_limits<double>::epsilon()

// Representacao da arvore de busca
typedef struct node
{
    std::vector<std::pair<int, int>> arcos_proibidos; // lista de arcos proibidos
    std::vector<double> multiplicadores;              // multiplicadores de lagrange do no
    std::vector<std::pair<int, int>> edges;           // 1-arvore solucao modificada do kruskal
    int maior_grau_i;                                 // indice do vertice com maior grau
    double LB = 0;                                    // valor da solucao
    int ID = 0;                                       // numero do no
    bool isFeasible = false;                          // true: solucao otima = UB
    bool pruning = false;                             // podar
} Node;

Node bnbComb(double **, size_t, double, int);

#endif // BNB_CNB_H_INCLUDED