#include "kruskal.h"
#include "bnb_cnb.h"
#include "sub_gradient.h"

/**
 * Funcao para calcular o somatorio dos custos das arestas
 * 
 * @param edges std::vector<pair<int,int>> arestas
 * @param C matriz de custo
 * @return double com custo acumulado das arestas
*/
double calculateCost(double **C, std::vector<pair<int, int>> edges)
{
    double custo = 0.0;
    for (auto &edge : edges)
    {
        custo += C[edge.first][edge.second];
    }

    return custo;
}

/**
 * Metodo do subgradiente para relaxacao lagrangiana do problema 
 * TSP simetrico.
 * 
 * 
 * @param node no
 * @param C matriz de custo
 * @param dim dimencao da matriz de custo
 * @param u vetor com os multiplicadores lagrangeanos
 * @param UB upper bound da heuristica
 * @param rho paramentro de controle do passo (default = 2)
 * @return struct Node com as informacoes do no
*/
void subgrad_method(Node &node, double **C, int dim, std::vector<double> u, double UB, double rho)
{
    // Numero maximo de iteracoes conscutivas sem aumento do valor de LB
    int p = 30;

    // contador de iteracoes conscutivas sem aumento do valor de LB
    int ite_fail = 0;

    // menor valor de rho
    double min_rho = rho / std::pow(2, 8);

    // best lower bound
    double bestLB = 0;

    // arestas da 1-arvore
    vii one_tree_edges, best_tree;

    // vertice de maior grau
    int maior_grau_pos = 0;

    while (rho > min_rho) // condicao de parada 1
    {
        // matriz de custo lagrangiana (_c)
        std::vector<std::vector<double>> _C(dim, std::vector<double>(dim));
        for (int i = 0; i < dim; ++i)
        {
            for (int j = 0; j < dim; ++j)
            {
                _C[i][j] = C[i][j] - u[i] - u[j];
                _C[j][i] = _C[i][j];
            }
        }

        // Rodar o Kruskal para gerar a 1-tree (bloking no vertice 0 por default)
        Kruskal kruskal(_C);

        // Resolver mst
        kruskal.MST(dim);

        // Converter mst para one-tree
        kruskal.mst_to_oneTree();            // menor isercao do vertice 0
        one_tree_edges = kruskal.getEdges(); // arestas da one-tree

        // subgradiente
        std::vector<int> mi(dim);
        std::fill(std::begin(mi), std::end(mi), 2); // grau 2
        for (auto &edge : one_tree_edges)
        {
            --mi[edge.first];
            --mi[edge.second];
        }

        // Lower Bound
        double LB = kruskal.cost;
        if (LB > bestLB)
        {
            bestLB = LB;

            // atualizando a melhor arvore
            best_tree = one_tree_edges;

            // vertice de maior grau
            std::vector<int>::iterator it = std::min_element(mi.begin(),
                                                             mi.end());
            maior_grau_pos = std::distance(mi.begin(), it);

            ite_fail = 0;
        }
        else // sem melhoria no LB
        {
            ++ite_fail;
            if (ite_fail >= p)
            {
                rho /= 2;
                ite_fail = 0;
            }
        }

        // varificando solucao otima
        if (std::ceil(bestLB) == UB) // condicao de parada 2
        {
            node.otimo = true;
            bestLB = UB;
            break;
        }
        bestLB = std::max(bestLB, LB);

        // calculando o passo
        double ssq = 0.0; // soma dos quadrados
        for (size_t i = 0; i < mi.size(); ++i)
        {
            ssq += mi[i] * mi[i];
        }

        double step = rho * (UB - LB) / ssq; // passo

        // Atualizacao dos multiplicadores
        for (size_t i = 0; i < u.size(); ++i)
        {
            u[i] += step * mi[i];
        }

        // condicao de parada 3
        if (ssq == 0) // solucao viavel
        {
            node.LB = calculateCost(C, one_tree_edges);
            node.is_upper_bound = true;
            break;
        }
    }

    // Att o node
    node.LB = bestLB;
    node.multiplicadores = u;
    node.one_tree = best_tree;
    node.maior_grau_i = maior_grau_pos;
}
