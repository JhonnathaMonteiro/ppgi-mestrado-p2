#include "movements.h"

struct melhor_movimento
{
    int f_s;   // Valor da funcao de avaliacao f(s)
    int i = 0; // Indice i do melhor movimento
    int j = 0; // Indice j do melhor movimento
};

/**
 * Realiza o movimeto de Swap na solucao s e retorna o melhor vizinho
 * 
 * @param s vetor solucao de entrada
 * @param **c matriz de adjacencia
 * @return s vetor solucao (melhor vizinho)
*/
std::vector<int> swap(std::vector<int> s, double f_s, double **c)
{
    int N = s.size(); // Tamanho do vetor solucao
    int _f_s;
    melhor_movimento mv_swap;
    mv_swap.f_s = f_s;

    for (int i = 1; i < N - 2; i++)
    {
        for (int j = i + 1; j < N - 1; j++)
        {
            _f_s = f_s; // Valor da solucao a ser avaliada

            // Nos adjacentes
            if (j - i == 1)
            {
                _f_s = _f_s - c[s[i - 1]][s[i]]; // Primeira quebra
                _f_s = _f_s - c[s[j]][s[j + 1]]; // Segunda quebra

                _f_s = _f_s + c[s[i - 1]][s[j]]; // Primeira adicao
                _f_s = _f_s + c[s[i]][s[j + 1]]; // Segunda adicao
            }
            // Nos nao adjacentes
            else
            {
                _f_s = _f_s - c[s[i - 1]][s[i]]; // Primeira quebra
                _f_s = _f_s - c[s[i]][s[i + 1]]; // Segunda quebra
                _f_s = _f_s - c[s[j - 1]][s[j]]; // Terceira quebra
                _f_s = _f_s - c[s[j]][s[j + 1]]; // Quarta quebra

                _f_s = _f_s + c[s[i - 1]][s[j]]; // Primeira adicao
                _f_s = _f_s + c[s[j]][s[i + 1]]; // Segunda adicao
                _f_s = _f_s + c[s[i]][s[j + 1]]; // Terceira adicao
                _f_s = _f_s + c[s[j - 1]][s[i]]; // Quarta adicao
            }

            // Checar melhora
            if (_f_s < mv_swap.f_s)
            {
                // Atualizar melhor movimento
                mv_swap.f_s = _f_s;
                mv_swap.i = i;
                mv_swap.j = j;
            }
        }
    }

    // Se nao teve melhoria
    if (mv_swap.i == mv_swap.j)
    {
        return s;
    }
    // Se teve melhoria
    else
    {
        // Construindo s
        std::swap(s[mv_swap.i], s[mv_swap.j]);
        return s;
    }
}

/**
 * Realiza o movimeto de 2-opt na solucao s e retorna o melhor vizinho
 * 
 * @param s vetor solucao de entrada
 * @param **c matriz de adjacencia
 * @return s vetor solucao (melhor vizinho)
*/
std::vector<int> two_opt(std::vector<int> s, double f_s, double **c)
{
    int N = s.size(); // Tamanho do vetor solucao
    int _f_s;
    melhor_movimento mv_2_opt;
    mv_2_opt.f_s = f_s;

    for (int i = 0; i < N - 2; i++)
    {
        for (int j = i + 2; j < N - 1; j++)
        {

            _f_s = f_s; // Valor da solucao a ser avaliada

            // Para funcionar com dados assimetricos
            // Invertendo sub
            for (int l = i + 1; l < j; l++)
            {
                _f_s = _f_s - c[s[l]][s[l + 1]] + c[s[l + 1]][s[l]]; // Invercao
            }

            _f_s = _f_s - c[s[i]][s[i + 1]]; // Primeira quebra
            _f_s = _f_s - c[s[j]][s[j + 1]]; // Segunda quebra

            _f_s = _f_s + c[s[i]][s[j]];         // Primeira adicao
            _f_s = _f_s + c[s[i + 1]][s[j + 1]]; // Segunda adicao

            // Checar melhora
            if (_f_s < mv_2_opt.f_s)
            {
                // Atualizar melhor movimento
                mv_2_opt.f_s = _f_s;
                mv_2_opt.i = i;
                mv_2_opt.j = j;
            }
        }
    }

    // Se nao teve melhoria
    if (mv_2_opt.i == mv_2_opt.j)
    {
        return s;
    }
    // Se teve melhoria
    else
    {
        // Construindo s
        std::reverse(s.begin() + mv_2_opt.i + 1, s.begin() + mv_2_opt.j + 1);

        return s;
    }
}

/**
 * Realiza o movimeto de Reinsertion ou Or-opt-2 ou Or-opt-3  na solucao s 
 * e retorna o melhor vizinho
 * 
 * @param s vetor solucao de entrada
 * @param **c matriz de adjacencia
 * @return s vetor solucao (melhor vizinho)
*/
std::vector<int> sub_reinsertion(std::vector<int> s, double f_s, double **c)
{
    int N = s.size(); // Tamanho do vetor solucao
    int _f_s;
    melhor_movimento mv_sub;
    mv_sub.f_s = f_s;

    // Geracao de numeros randomicos baseados na distribuicao uniforme <int>
    std::random_device rseed;
    std::mt19937 rgen(rseed());
    std::uniform_int_distribution<int> idist(0, 2); // intervalo [0,2] <int>

    int sub_size = idist(rgen);
    // 0 - Reinsertion
    // 1 - Or-opt-2
    // 2 - Or-opt-3

    for (int i = 1; i < N - 2 - sub_size; i++)
    {
        for (int j = i + 1 + sub_size; j < N - 1; j++)
        {
            _f_s = f_s; // Valor da solucao a ser avaliada

            _f_s = _f_s - c[s[i - 1]][s[i]];        // Primeira quebra
            _f_s = _f_s - c[s[i + sub_size]][s[j]]; // Segunda quebra
            _f_s = _f_s - c[s[j]][s[j + 1]];        // Terceira quebra

            _f_s = _f_s + c[s[i - 1]][s[j]];            // Primeira adicao
            _f_s = _f_s + c[s[j]][s[i]];                // Segunda adicao
            _f_s = _f_s + c[s[i + sub_size]][s[j + 1]]; // Terceira adicao

            // Checar melhora
            if (_f_s < mv_sub.f_s)
            {
                // Atualizar melhor movimento
                mv_sub.f_s = _f_s;
                mv_sub.i = i;
                mv_sub.j = j;
            }
        }
    }

    // Se nao teve melhoria
    if (mv_sub.i == mv_sub.j)
    {
        return s;
    }
    // Se teve melhoria
    else
    {
        // Construindo s
        std::rotate(s.begin() + mv_sub.i,
                    s.begin() + mv_sub.j,
                    s.begin() + mv_sub.j + 1);

        return s;
    }
}
