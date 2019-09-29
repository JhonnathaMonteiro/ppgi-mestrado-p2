#include "bnb_cnb.h"
#include "hungarian.h"
#include "mtx_conv.h"
#include "data.h"

/*
* Funcao para calcular a solucao do node e atualiza sua estrutura
*/
void calcularSolucao(Node &node, Data *data)
{
    // TODO: Retirar isso de dentro da funcao
    double **cost = new double *[data->getDimension()];
    for (int i = 0; i < data->getDimension(); i++)
    {
        cost[i] = new double[data->getDimension()];
        for (int j = 0; j < data->getDimension(); j++)
        {
            cost[i][j] = data->getDistance(i, j);
        }
    }

    //verificar arcos proibidos e modificar a matriz de custo de acordo
    if (!node.arcos_proibidos.empty())
    {
        for (auto &i : node.arcos_proibidos)
        {
            //tsp simetrico
            cost[i.first][i.second] = INFINITE;
            cost[i.second][i.first] = INFINITE;
        }
    }

    hungarian_problem_t p;
    int mode = HUNGARIAN_MODE_MINIMIZE_COST;
    hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode); // Carregando o problema

    //resolvendo o no
    node.lower_bound = hungarian_solve(&p); //lower bound

    //gerando os subtours a partir da matriz solucao (A) do alg. hungaro
    node.subtour = subConstructor(p);

    //escolhendo o subtour
    //criterio: menor quantidade de nos (empate: menor indice)
    int tamanho = __INT_MAX__; // escolha inicial (inf)
    for (int i = 0; i < node.subtour.size(); i++)
    {
        if (node.subtour[i].size() < tamanho)
        {
            tamanho = node.subtour[i].size();
            node.escolhido = i;
        }
    }

    //verificar se a solucao e viavel
    if (node.subtour.size() == 1)
    {
        //solucao viavel
        node.podar = true;
    }
}

/*
* Branch and bound combinatorio
*/
Node bnbComb(std::list<Node> arvore, Data *data)
{
    //inicializando a solucao
    Node bestNode;
    bestNode.lower_bound = INFINITE;
    Node currentNode;

    // Busca em largura
    while (!arvore.empty())
    {
        currentNode = arvore.front();

        //verificar se eh candidato a solucao (podar)
        if (currentNode.podar)
        {
            if (currentNode.lower_bound < bestNode.lower_bound)
            {
                //atualizar o melhor node
                bestNode = currentNode;
            }
        }
        // caso contrario gerar nos (branch)
        else
        {
            int escolhidoSize = currentNode.subtour[currentNode.escolhido].size();

            int aux = escolhidoSize == 3 ? 2 : 1;

            //iterar por todos os arcos do subour escolhido
            for (int i = 0; i < escolhidoSize - aux; i++)
            {
                Node n;
                n.arcos_proibidos = currentNode.arcos_proibidos;

                std::pair<int, int> arco;
                arco.first = currentNode.subtour[currentNode.escolhido][i];
                arco.second = currentNode.subtour[currentNode.escolhido][i + 1];
                n.arcos_proibidos.push_back(arco);

                // n.arcos_proibidos.push_back(arco_proibido);
                calcularSolucao(n, data);

                arvore.push_back(n); //inserir novos nos na arvore
            }
        }

        //remover o pai
        arvore.pop_front();
    }

    return bestNode;
}