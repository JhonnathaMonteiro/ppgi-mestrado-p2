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
    for (int i = 0; i < (int)node.subtour.size(); i++)
    {
        if ((int)node.subtour[i].size() < tamanho)
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
Node bnbComb(std::list<Node> arvore, Data *data, size_t dim, double upper_bound, int busca)
{

    //inicializando a solucao
    Node bestNode;
    bool otimo = false;
    bestNode.lower_bound = INFINITE;
    Node currentNode;

    // Busca em largura
    while (!arvore.empty())
    {
        //verificar solucao otima
        if (otimo)
        {
            break;
        }

        //selecionando o no que sera analisado
        //criterio: no com o menor LB
        double menorLB = INFINITE;
        std::list<Node>::iterator currPos; //posicao do no atual na arvore
        std::list<Node>::iterator it;      //iterador

        switch (busca)
        {
        case BUSCA_BEST_BOUND:
            for (it = arvore.begin(); it != arvore.end(); ++it)
            {
                if (it->lower_bound < menorLB)
                {
                    menorLB = it->lower_bound;
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
            //Pegar o primeiro no da arvore
            it = arvore.begin();
            currPos = it;
            currentNode = *it;
            break;

        default:
            break;
        }

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
        for (int i = 0; i < escolhidoSize - 1; i++)
        {

            std::pair<int, int> arco;
            arco.first = currentNode.subtour[currentNode.escolhido][i];
            arco.second = currentNode.subtour[currentNode.escolhido][i + 1];

            // proibindo o arco
            Node N1;
            N1.arcos_proibidos = currentNode.arcos_proibidos;
            N1.arcos_proibidos.push_back(arco);
            calcularSolucao(N1, cost, dim);

            if (N1.podar) //encontrou um upper bound
            {
                if (N1.lower_bound < bestNode.lower_bound)
                {
                    //atualizar o melhor node
                    bestNode = N1;

                    // examina se e possivel pode podar alguns nós por limite da arvore
                    for (it = arvore.begin(); it != arvore.end(); ++it)
                    {
                        if (it->lower_bound > N1.lower_bound)
                        {
                            //remover no
                            arvore.erase(it);
                        }
                    }

                    if (bestNode.lower_bound == upper_bound)
                    {
                        otimo = true;
                        break;
                    }
                }
            }

            // valores maiores que o upper_bound nao devem ser explorados
            if (N1.lower_bound < upper_bound)
            {
                arvore.push_back(N1);
            }
        }
        //remover o no atual
        arvore.erase(currPos);
    }
    return bestNode;
}
