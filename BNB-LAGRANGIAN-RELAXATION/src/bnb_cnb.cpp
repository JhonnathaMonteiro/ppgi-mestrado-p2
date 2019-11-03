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
void calcularSolucao(Node &node, double **cost, int dim, double UB)
{

    //verificar arcos proibidos/obrigatorios e modificar a matriz de custo de acordo
    if (!node.arcos_proibidos.empty())
    {
        for (auto &arco : node.arcos_proibidos)
        {
            cost[arco.first][arco.second] = INFINITE;
        }
    }

    subgrad_method(node, cost, dim, node.multiplicadores, UB, 2); // rho = 2
}

/*
* Branch and bound combinatorio
*/
Node bnbComb(double **cost, int dim, double heuristic_UB, int busca)
{
    //raiz
    Node root_node;

    // multiplicadores da no raiz {0, ... , 0}
    std::vector<double> u(dim);
    std::fill(std::begin(u), std::end(u), 0);
    root_node.multiplicadores = u;

    //inicializando arvore
    std::list<Node> arvore;

    //resolvendo a raiz
    // void calcularSolucao(Node &node, double **cost, int dim, double UB)
    calcularSolucao(root_node, cost, dim, heuristic_UB);

    arvore.push_back(root_node);

    //inicializando a solucao
    Node bestNode, currentNode;
    bestNode.LB = heuristic_UB;

    while (!arvore.empty())
    {

        //metodo de busca
        double menorLB = INFINITE;
        std::list<Node>::iterator currPos; //posicao do no atual na arvore
        std::list<Node>::iterator it;      //iterador

        switch (busca)
        {
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

        case BUSCA_EM_PROFUNDIDADE:
            //Pegar o no mais profundo na arvore
            it = --arvore.end(); //-- para o iterador ficar na posicao do ultimo
            currPos = it;
            currentNode = *it;
            break;

        case BUSCA_EM_LARGURA:
            //Pegar o no mais profundo na arvore
            it = arvore.begin();
            currPos = it;
            currentNode = *it;
            break;

        default:
            break;
        }

        // Verificar se o no atingiu a solucao otima
        if (currentNode.otimo)
        {
            bestNode = currentNode;
            break;
        }

        if (!currentNode.is_upper_bound)
        {

            // Determinar os arcos proibidos
            std::vector<pair<int, int>> arcos_p;
            for (auto &edge : currentNode.one_tree)
            {
                if (edge.first == currentNode.maior_grau_i || edge.second == currentNode.maior_grau_i)
                {
                    arcos_p.push_back(edge);
                }
            }

            // Iterando pelos arcos_p e branching
            for (auto &arco : arcos_p)
            {
                // criando node filho
                Node N1;
                N1.arcos_proibidos = currentNode.arcos_proibidos; //herda os arcos
                N1.multiplicadores = currentNode.multiplicadores; //herda os multiplicadores
                N1.arcos_proibidos.push_back(arco);
                calcularSolucao(N1, cost, dim, bestNode.LB);

                // verificando se o no deve ser adicionado na arvore
                // valores maiores que o melhor UB (bestNode.LB)
                // nao devem ser explorados
                if (N1.LB <= bestNode.LB)
                {
                    arvore.push_back(N1);

                    //verificando se foi encontrado um upper_bound (solucao viavel)
                    if (N1.is_upper_bound)
                    {

                        // atualizando o UB
                        bestNode = N1;

                        //verificando solucao otima

                        // examinar se eh possivel pode podar alguns nos por limite
                        // da arvore
                        for (it = arvore.begin(); it != arvore.end(); ++it)
                        {
                            if (it->LB > N1.LB)
                            {
                                //remover no
                                arvore.erase(it);
                            }
                        }
                    }
                }
            }
        }
        else if (currentNode.LB < bestNode.LB)
        {
            bestNode = currentNode;
        }
        //remover o no atual
        arvore.erase(currPos);
    }
    return bestNode;
}
