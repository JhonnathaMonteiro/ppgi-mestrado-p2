#include "hungarian.h"
#include "mtx_conv.h"

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