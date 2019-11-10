#include "kruskal.h"
#include "bnb_cnb.h"
#include "sub_gradient.h"

/**
 * Metodo do subgradiente para relaxacao lagrangiana do problema 
 * TSP simetrico.
 * 
 * 
 * @param node no
 * @param C matriz de custo
 * @param dim dimencao da matriz de custo
 * @param lambda vetor com os multiplicadores lagrangeanos
 * @param UB upper bound
*/
void subgrad_method(Node &node,
                    double **C,
                    size_t dim,
                    double UB)
{
    int fail_count = 0;                                // Contador de falhas
    std::vector<double> lambda = node.multiplicadores; // multiplicadores
    double sigma = 1;                                  // Parametro de refinamento dos multiplicadores
    double sigma_lim = 1.0 / 16364.0;                  // Menor valor permitido de sigma
    double LB;
    double treeCost;

    std::vector<double> vetorInfinito1(node.arcos_proibidos.size(), std::numeric_limits<double>::infinity());
    std::vector<double> vetorInfinito2(node.arcos_proibidos.size(), std::numeric_limits<double>::infinity());

    int count = 0;
    for (const auto &arco : node.arcos_proibidos)
    {
        std::swap(C[arco.first][arco.second], vetorInfinito1[count]);
        std::swap(C[arco.second][arco.first], vetorInfinito2[count]);
        count++;
    }

    //verificar arcos proibidos e modificar a matriz de custo de acordo
    if (!node.arcos_proibidos.empty())
    {
        for (auto &arco : node.arcos_proibidos)
        {
            C[arco.first][arco.second] = INFINITE;
            C[arco.second][arco.first] = INFINITE;
        }
    }

    while (!node.isFeasible)
    {
        // Primeiro passo: encontrar a min 1-tree
        std::vector<std::vector<double>> _C(dim, std::vector<double>(dim));
        for (size_t row = 0; row < dim; ++row)
        {
            for (size_t col = 0; col < dim; ++col)
            {
                _C[row][col] = C[row][col] - lambda[row] - lambda[col];
            }
        }
        Kruskal kruskal(_C);
        treeCost = kruskal.MST(dim);
        vii oneTree = kruskal.getEdges(); // arestas da one-tree

        // Segundo passo: encontrar o LB
        double lambda_sum = 0.0;
        for (auto &lambda_i : lambda)
        {
            lambda_sum += lambda_i;
        }
        LB = treeCost + 2 * lambda_sum;
        if ((LB - node.LB) <= EPSILON)
        {
            ++fail_count;
        }
        if (fail_count == 30)
        {
            sigma /= 2;
            fail_count = 0;
        }

        // Terceiro passo: verificar se encontrou solucao viavel
        std::vector<int> subgrad(dim, 2); // vetor subgradiente
        for (auto &edge : oneTree)
        {
            --subgrad[edge.first];
            --subgrad[edge.second];
        }
        std::vector<int>::iterator it = std::min_element(subgrad.begin(),
                                                         subgrad.end());

        // Att node
        node.maior_grau_i = std::distance(subgrad.begin(), it);
        node.one_tree = oneTree;
        node.LB = LB;
        node.multiplicadores = lambda;

        if (*it == 0) // Solucao viavel
        {
            node.isFeasible = true;
        }

        if (node.isFeasible || std::abs(LB - UB) <= 0.000001)
        {
            node.pruning = true;
            break;
        }

        // Quarto passo: Att multiplicadores
        double norma2 = 0.0;
        for (size_t i = 0; i < dim; ++i)
        {
            norma2 += subgrad[i] * subgrad[i];
        }
        double step = sigma * (UB - LB) / norma2;
        for (size_t i = 0; i < dim; ++i)
        {
            lambda[i] += step * subgrad[i];
        }

        if (sigma < sigma_lim)
        {
            break;
        }
    }

    count = 0;
    for (const auto &arco : node.arcos_proibidos)
    {
        std::swap(C[arco.first][arco.second], vetorInfinito1[count]);
        std::swap(C[arco.second][arco.first], vetorInfinito2[count]);
        count++;
    }
}