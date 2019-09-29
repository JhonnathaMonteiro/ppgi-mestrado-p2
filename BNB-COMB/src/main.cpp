#include <iostream>
#include <vector>
#include <utility>
#include <list>
#include <numeric>
#include <algorithm>
#include <limits.h>
using namespace std;

#include "data.h"
#include "hungarian.h"

// Representacao da arvore de busca
typedef struct node
{
    std::vector<pair<int, int>> arcos_proibidos; // lista de arcos proibidos
    std::vector<std::vector<int>> subtour;       // conjunto de subtours da solucao
                                                 // gerada pelo alg. hungaro
    double lower_bound;                          //lower_bound produzido pelo no (ou custo total da
                                                 //solucao do algoritmo hungaro)
    int escolhido;                               // subtour escolhido dado o criterio de selecao
    bool podar = false;                          // variavel que diz se o no deve gerar filhos
} Node;

void eraseByValue(std::vector<int> &vec, int val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

/*
* Funcao para transformar a matriz A em um std::vector<std::vector<int>> 
*/
std::vector<std::vector<int>> subConstructor(hungarian_problem_t p)
{
    std::vector<std::vector<int>> subs;
    std::vector<int> sub;

    // Vetor auxiliar
    std::vector<int> aux(p.num_rows);
    std::iota(aux.begin(), aux.begin() + p.num_rows, 0);

    int linha = aux[0];
    while (!aux.empty())
    {
        eraseByValue(aux, linha);
        sub.push_back(linha);

        for (int i = 0; i < p.num_cols; i++)
        {
            if (p.assignment[linha][i] == 1)
            {
                if (sub[0] == i)
                {
                    sub.push_back(i);
                    subs.push_back(sub);
                    sub.clear();
                    eraseByValue(aux, linha);
                    linha = aux[0];
                }
                else
                {
                    auto pos = std::find(aux.begin(), aux.end(), i);
                    linha = aux[std::distance(aux.begin(), pos)];
                }
                break;
            }
        }
    }

    return subs;
}

/*
* Funcao para calcular a solucao do node e atualiza sua estrutura
*/
void calcularSolucao(Node &node, Data *data)
{

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

    // Busca em largura
    while (!arvore.empty())
    {
        //verificar se eh candidato a solucao (podar)
        if (arvore.front().podar)
        {
            if (arvore.front().lower_bound < bestNode.lower_bound)
            {
                //atualizar o melhor node
                bestNode = arvore.front();
            }
        }
        // caso contrario gerar nos (branch)
        else
        {
            int escolhidoSize = arvore.front().subtour[arvore.front().escolhido].size();
            for (int i = 0; i < escolhidoSize - 2; i++)
            {
                Node n;
                n.arcos_proibidos = arvore.front().arcos_proibidos;

                std::pair<int, int> arco;
                arco.first = arvore.front().subtour[arvore.front().escolhido][i];
                arco.second = arvore.front().subtour[arvore.front().escolhido][i + 1];
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

int main(int argc, char **argv)
{
    //leitura da instancia
    Data *data = new Data(argc, argv[1]);
    data->readData();

    // criacao da arvore de busca
    std::list<Node> arvore;

    Node raiz;
    calcularSolucao(raiz, data); // calcular solucao usando algoritmo hungaro e matriz
                                 // de distancia inicial e preencher os atributos da
                                 // estrutura  Node de acordo

    arvore.push_back(raiz);

    Node solucao = bnbComb(arvore, data);
    // calcularSolucao(raiz, data);

    std::cout << "Solucao: " << solucao.lower_bound << std::endl;

    return 0;
}