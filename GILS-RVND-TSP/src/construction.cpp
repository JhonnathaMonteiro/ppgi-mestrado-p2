#include "construction.h"

using namespace std;

struct InsertionInfo
{
    int noInserido;
    int arestaRemovida;
    double custo;
};

bool compareByCusto(const InsertionInfo &a, const InsertionInfo &b)
{
    return a.custo < b.custo;
};

void eraseByValue(std::vector<int> &vec, int val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

// std::vector<int> construction(int N, double alpha, double c[18][18])
std::vector<int> construction(int N, double alpha, double **c)
{
    // c = matriz de custo
    // N = dimensao

    std::vector<int> s = {1, 1};                                      //subtour inicial
    std::vector<int> listaDeCandidatos(N - 1);                        //cadidatos
    std::iota(listaDeCandidatos.begin(), listaDeCandidatos.end(), 2); //gerando a lista de cadidatos

    std::vector<InsertionInfo>
        custoInsercao((s.size() - 1) * listaDeCandidatos.size());

    for (int m = 0; m < (N - 1); m++)
    {

        for (int i = 0, j = 1, l = 0; i < s.size() - 1; i++, j++)
        {
            for (int k : listaDeCandidatos)
            {
                custoInsercao[l].custo = c[s[i]][k] + c[s[j]][k] - c[s[i]][s[j]];
                custoInsercao[l].noInserido = k;
                custoInsercao[l].arestaRemovida = i;
                l++;
            }
        }

        // sorting custoInsercao
        std::sort(custoInsercao.begin(), custoInsercao.end(), compareByCusto);

        // Escolher randomicamente um dos alphaN primeiros elementos de custoInsercao
        int alphaN = std::round(alpha * (listaDeCandidatos.size() - 1));

        // Geracao de numeros randomicos baseados na distribuicao uniforme <int>
        std::random_device rseed;
        std::mt19937 rgen(rseed());
        std::uniform_int_distribution<int> idist(0, alphaN);

        // Elemento escolhido n
        int n = idist(rgen);

        // Inserindo elemento no subtour
        s.insert(s.begin() + custoInsercao[n].arestaRemovida + 1, custoInsercao[n].noInserido);

        // Remover da listaDeCandidatos o candidato inserido em s
        eraseByValue(listaDeCandidatos, custoInsercao[n].noInserido);

        // Resize o tamanho do vetor custoInsercao
        custoInsercao.resize((s.size() - 1) * listaDeCandidatos.size());
    }

    return s;
}
