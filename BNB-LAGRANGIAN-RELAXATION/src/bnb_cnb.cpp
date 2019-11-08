#include "bnb_cnb.h"
#include "data.h"
#include "sub_gradient.h"

void _eraseByValue(std::vector<int> &vec, int val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

/*
* Funcao para calcular a solucao do node e atualiza sua estrutura
*/
void calcularSolucao(Node &node, double **cost, size_t dim, double UB)
{

    //verificar arcos proibidos e modificar a matriz de custo de acordo
    if (!node.arcos_proibidos.empty())
    {
        for (auto &arco : node.arcos_proibidos)
        {
            cost[arco.first][arco.second] = INFINITE;
            cost[arco.second][arco.first] = INFINITE;
        }
    }

    subgrad_method(node, cost, (size_t)dim, node.multiplicadores, UB);
}

/**
 * Branch and bound combinatorio
 * 
 * @param cost ** matriz de custo
 * @param dim dimencao da matriz de custo
 * @param UB upper bound da heuristica
 * @param busca int com o tipo de busca
 * 
 * @return o melhor node encontrado 
 */
Node bnbComb(double **cost, size_t dim, double UB, int busca)
{
    // raiz
    Node root_node;

    // multiplicadores do no raiz {0, ... , 0}
    std::vector<double> u(dim);
    root_node.multiplicadores = u;

    // inicializando arvore
    std::list<Node> arvore;

    // resolvendo a raiz
    calcularSolucao(root_node, cost, dim, UB);
    arvore.push_back(root_node);

    // inicializando a solucao
    Node bestNode, currentNode;
    bestNode = root_node;

    while (!arvore.empty())
    {

        // Metodo de busca
        double menorLB = INFINITE;
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

        //PRINT NODE
        std::cout << "CurrentNode LB:" << currentNode.LB << std::endl;

        // Solucao viavel
        if (currentNode.isFeasible)
        {
            // solucao otima
            if (currentNode.LB == UB)
            {
                bestNode = currentNode;
                break;
            }

            // Examinando se eh possivel pode podar alguns nos por limite
            for (it = arvore.begin(); it != arvore.end(); ++it)
            {
                if (it->LB > currentNode.LB)
                {
                    arvore.erase(it);
                }
            }

            // Verificar se possui solucao melhor que o bestNode
            if (currentNode.LB < UB)
            {
                UB = currentNode.LB;
                bestNode = currentNode;
                arvore.erase(currPos);
                continue;
            }
        }

        // Determinando os Arcos Proibidos
        std::vector<pair<int, int>> arcos_p;
        for (auto &edge : currentNode.one_tree)
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
            N1.arcos_proibidos.push_back(arco);
            calcularSolucao(N1, cost, dim, UB);
            if (N1.LB != 0)
            {
                arvore.push_back(N1);
            }
        }

        //remover o no atual
        arvore.erase(currPos);
    }
    return bestNode;
}
