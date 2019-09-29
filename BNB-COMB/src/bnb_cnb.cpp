#include "bnb_cnb.h"
#include "hungarian.h"
#include "mtx_conv.h"
#include "data.h"

void _eraseByValue(std::vector<int> &vec, int val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

/*
* Funcao para calcular a solucao do node e atualiza sua estrutura
*/
void calcularSolucao(Node &node, double **cost, int dim)
{

    //verificar arcos proibidos/obrigatorios e modificar a matriz de custo de acordo
    if (!node.arcos_proibidos.empty())
    {
        for (auto &arco : node.arcos_proibidos)
        {
            cost[arco.first][arco.second] = INFINITE;
        }
    }
    if (!node.arcos_obrigatorios.empty())
    {
        double valorExtra = 0; //valor a ser adicionado na solucao do subproblema
        //linhas e colunas excluidas no subproblema
        std::vector<int> linhasExcluidas;
        std::vector<int> colunasExcluidas;

        std::vector<int> new_rows(dim);
        std::vector<int> new_cols(dim);
        std::iota(new_rows.begin(), new_rows.begin() + dim, 0);
        std::iota(new_cols.begin(), new_cols.begin() + dim, 0);

        //resolver uma subproblema menor, onde o arco eh obrigatorio
        for (auto &arco : node.arcos_obrigatorios)
        {

            //atualizando os valores de new_rowls e new_cols
            _eraseByValue(new_rows, arco.first);
            _eraseByValue(new_cols, arco.second);
            valorExtra += cost[arco.first][arco.second];
        }

        //Mapeando os vetores com a nova matriz de custo
        double **new_cost = new double *[new_rows.size()];
        for (int i = 0; i < new_rows.size(); i++)
        {

            new_cost[i] = new double[new_rows.size()];
            for (int j = 0; j < new_rows.size(); j++)
            {
                new_cost[i][j] = cost[new_rows[i]][new_cols[j]];
            }
        }

        //nova matriz de custo
        for (int i = 0; i < dim; i++)
        {
            delete[] cost[i];
        }
        delete[] cost;

        double **cost = new double *[new_cols.size()];
        cost = new_cost;
    }

    hungarian_problem_t p;
    int mode = HUNGARIAN_MODE_MINIMIZE_COST;
    hungarian_init(&p, cost, dim, dim, mode); // Carregando o problema

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
Node bnbComb(std::list<Node> arvore, Data *data, int dim)
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
            //matriz de custo
            // TODO: Retirar isso de dentro da funcao
            double **cost = new double *[dim];
            for (int i = 0; i < dim; i++)
            {
                cost[i] = new double[dim];
                for (int j = 0; j < dim; j++)
                {
                    cost[i][j] = data->getDistance(i, j);
                }
            }

            int escolhidoSize = currentNode.subtour[currentNode.escolhido].size();

            //iterar por todos os arcos do subtour escolhido
            //TODO: utilizar held_karp
            for (int i = 0; i < escolhidoSize - 2; i++)
            {

                std::pair<int, int> arco;
                arco.first = currentNode.subtour[currentNode.escolhido][i];
                arco.second = currentNode.subtour[currentNode.escolhido][i + 1];

                // Arco excluido
                Node N1;
                N1.arcos_proibidos = currentNode.arcos_proibidos;
                N1.arcos_obrigatorios = currentNode.arcos_obrigatorios;
                N1.arcos_proibidos.push_back(arco);
                calcularSolucao(N1, cost, dim);

                // Arco incluso
                Node N2;
                N2.arcos_proibidos = currentNode.arcos_proibidos;
                N2.arcos_obrigatorios = currentNode.arcos_obrigatorios;
                N2.arcos_obrigatorios.push_back(arco);
                calcularSolucao(N2, cost, dim);

                //inserir novos nos na arvore
                arvore.push_back(N1);
                arvore.push_back(N2);
            }
        }

        //remover o pai
        arvore.pop_front();
    }

    return bestNode;
}
