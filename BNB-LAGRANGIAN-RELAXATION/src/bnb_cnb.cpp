#include <iomanip>
#include <iostream>
#include <string>

#include "bnb_cnb.h"
#include "data.h"
#include "sub_gradient.h"

void printNode(const Node &node, const std::list<Node> &arvore, const double &UB)
{
    // Header
    if (arvore.size() % 100 == 0 || arvore.size() == 1)
    {
        std::cout << std::string(80, '*') << std::endl;
        std::cout << std::right;
        std::cout << std::setw(10) << "TreeSize";
        std::cout << std::setw(10) << "NodeID";
        std::cout << std::setw(15) << "Feasible";
        std::cout << std::setw(15) << "Pruning";
        std::cout << std::setw(15) << "Z(LD)";
        std::cout << std::setw(15) << "UB";
        std::cout << std::endl;
        std::cout << std::string(80, '*') << std::endl;
    }

    std::cout << std::setw(10) << arvore.size();
    std::cout << std::setw(10) << node.ID;
    std::cout << std::boolalpha;
    std::cout << std::setw(15) << node.isFeasible;
    std::cout << std::setw(15) << node.pruning;
    std::cout << std::noboolalpha;
    std::cout << std::setw(15) << node.LB;
    std::cout << std::setw(15) << UB;
    std::cout << std::endl;
}

/**
 * Funcao para calcular o somatorio dos custos das arestas
 * 
 * @param edges std::vector<pair<int,int>> arestas
 * @param C matriz de custo
 * @return double com custo acumulado das arestas
*/
double calculateCost(double **C, std::vector<pair<int, int>> edges)
{
    double val = 0.0;
    for (auto &edge : edges)
    {
        val += C[edge.first][edge.second];
    }

    return val;
}

/**
 * Branch and bound
 * 
 * @param C ** matriz de custo
 * @param dim dimencao da matriz de custo
 * @param UB upper bound da heuristica
 * @param busca int com o tipo de busca
 * 
 * @return o melhor node encontrado 
 */
Node bnbComb(double **C, size_t dim, double UB, int busca)
{
    // Raiz
    Node root_node;

    // Multiplicadores do no raiz {0, ... , 0}
    std::vector<double> u(dim);
    root_node.multiplicadores = u;

    // Inicializando arvore
    std::list<Node> arvore;

    // Contador de nos
    int nodeCount = 1;
    root_node.ID = nodeCount;

    // resolvendo a raiz
    subgrad_method(root_node, C, dim, UB);
    arvore.push_back(root_node);

    // inicializando a solucao
    Node bestNode, currentNode;
    bestNode = root_node;

    while (!arvore.empty())
    {

        // Metodo de busca
        double menorLB = std::numeric_limits<double>::infinity();
        std::list<Node>::iterator currPos; //posicao do no atual na arvore
        std::list<Node>::iterator it;      //iterador

        switch (busca)
        {
        // Pegar o melhor na arvore
        case BUSCA_BEST_BOUND:
            for (it = arvore.begin(); it != arvore.end(); ++it)
            {
                if (it->LB < menorLB)
                {
                    menorLB = it->LB;
                    currPos = it;
                    currentNode = *it;
                }
            }
            break;

        // Pegar o no mais profundo na arvore
        case BUSCA_EM_PROFUNDIDADE:
            it = --arvore.end(); //-- para o iterador ficar na posicao do ultimo
            currPos = it;
            currentNode = *it;
            break;

        // Pegar o primeiro no na arvore
        case BUSCA_EM_LARGURA:
            it = arvore.begin();
            currPos = it;
            currentNode = *it;
            break;

        default:
            break;
        } // Fim Metodo de Busca

        //PRINTING NODE
        printNode(currentNode, arvore, UB);

        // Solucao viavel
        if (currentNode.isFeasible)
        {
            // Calculando custo real
            currentNode.LB = calculateCost(C, currentNode.edges);

            // Solucao otima
            if (std::abs(UB - currentNode.LB) <= EPSILON)
            {
                bestNode = currentNode;
                break;
            }

            // Examinando se eh possivel pode podar alguns nos por limite
            for (it = arvore.begin(); it != arvore.end(); it++)
            {
                if (it->LB > currentNode.LB)
                {
                    it->pruning = true;
                }
            }

            // Verificar se possui solucao melhor que o bestNode
            if (currentNode.LB < UB)
            {
                UB = currentNode.LB;
                bestNode = currentNode;
            }
        }

        if (!currentNode.pruning)
        {
            // acho que isso so funciona pra busca best_first
            if (std::ceil(currentNode.LB) == UB)
            {
                bestNode = currentNode;
                break;
            }

            // Determinando os Arcos Proibidos
            std::vector<std::pair<int, int>> arcos_p;
            for (auto &edge : currentNode.edges)
            {
                if (edge.first == currentNode.maior_grau_i || edge.second == currentNode.maior_grau_i)
                {
                    arcos_p.push_back(edge);
                }
            }

            // Iterando por arcos_p e proibindo um arco em cada branching
            for (auto &arco : arcos_p)
            {
                // Criando node filho
                Node N1;
                N1.arcos_proibidos = currentNode.arcos_proibidos; // herda os arcos
                N1.multiplicadores = currentNode.multiplicadores; // herda os multiplicadores
                N1.ID = nodeCount;
                N1.arcos_proibidos.push_back(arco);

                subgrad_method(N1, C, dim, UB);

                arvore.push_back(N1);
                ++nodeCount;
            }
        }

        // Remover no atual
        arvore.erase(currPos);
    }
    return bestNode;
}
