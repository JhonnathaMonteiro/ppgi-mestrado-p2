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
                    std::vector<double> lambda,
                    double UB)
{
    int Tmax = 2000;    // Numero maximo de iteracoes
    int count = 1;      // Contador de iteracoes
    int Tlim = 10;      // Numero maximo de falhas consecutivas (em melhorar o LB)
    int fail_count = 0; // Contador de falhas consecutivas

    double sigma = 2; // Parametro de refinamento dos multiplicadores
    double LB = 0;    // LB inicial igual
    node.LB = LB;

    while (!node.isFeasible && LB < UB && count < Tmax)
    {
        // Primeiro passo: encontrar a min 1-tree
        // com: _c(i,j) = c(i,j) - lambda(i) - lambda(j)
        std::vector<std::vector<double>> _C(dim, std::vector<double>(dim));
        for (size_t row = 0; row < dim; ++row)
        {
            for (size_t col = 0; col < dim; ++col)
            {
                _C[row][col] = C[row][col] - lambda[row] - lambda[col];
            }
        }
        Kruskal kruskal(_C);
        kruskal.MST(dim);
        kruskal.mst_to_oneTree();         // menor isercao do vertice 0
        vii oneTree = kruskal.getEdges(); // arestas da one-tree

        // Segundo passo: encontrar o LB
        double lambda_sum = 0.0;
        for (auto &lambda_i : lambda)
        {
            lambda_sum += lambda_i;
        }
        LB = kruskal.cost + 2 * lambda_sum;
        if (LB > node.LB) // melhora no LB ?
        {
            fail_count = 0;
        }
        else
        {
            ++fail_count;
        }

        // Terceiro passo: verificar se encontrou solucao viavel
        std::vector<int> subgrad = kruskal.getSubGrad();
        std::vector<int>::iterator it = std::min_element(subgrad.begin(),
                                                         subgrad.end());

        if (*it == 0) // Solucao viavel
        {
            // Att node
            node.maior_grau_i = std::distance(subgrad.begin(), it);
            node.one_tree = oneTree;
            node.LB = LB;
            node.isFeasible = true;
            node.multiplicadores = lambda;
        }
        else
        {

            if (fail_count == 0 && LB < UB)
            {
                // Att node
                node.maior_grau_i = std::distance(subgrad.begin(), it);
                node.one_tree = oneTree;
                node.LB = LB;
                node.multiplicadores = lambda;
            }

            // Quarto passo: Att multiplicadores
            double norma2 = 0.0;
            for (size_t i = 0; i < dim; i++)
            {
                norma2 += subgrad[i] * subgrad[i];
            }
            double step = sigma * (UB - LB) / norma2;
            for (size_t i = 0; i < dim; ++i)
            {
                lambda[i] += step * subgrad[i];
            }
        }

        if (count % 30 == 0) // divide sigma a cada 30 iteracoes
        // if (fail_count > Tlim)
        {
            sigma /= 2;
            fail_count = 0;
        }
        ++count;
    }
}