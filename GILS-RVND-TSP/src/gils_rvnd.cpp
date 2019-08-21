#include "gils_rvnd.h"
#include "construction.h"
#include "movements.h"
#include "utils.h"

// Geracao de numeros randomicos baseados na distribuicao uniforme
std::random_device rseed;
std::mt19937 rgen(rseed());

double f(std::vector<int> &, double **);
void eraseByValue2(std::vector<int> &, int);
std::vector<int> rvnd(std::vector<int>, double **);
std::vector<int> pertub(std::vector<int>);

/**
 * Heuristica GILS-RVND 
 * 
*/
std::vector<int> gils_rvnd(int Imax, int Iils, int dimension, double **c)
{
    // s
    // s_ = s'
    // _s_ = s*
    std::vector<int> s, s_, _s_;

    // s_ <-- s
    int iterILS;

    // f <-- infinito
    double f_ = std::numeric_limits<double>::max();

    for (int i = 0; i < Imax; i++)
    {

        std::uniform_real_distribution<double> R(0.0, 1.0); // [0.0, 1.0] <double>

        // Escolher randomicamente um valor alpha no range [0,1] em R
        // alpha <-- randon value of R
        double alpha = R(rgen);

        // Construcao da solucao inicial
        s = construction(dimension, alpha, c);

        // s_ <-- s
        s_ = s;

        // iterILS <--0
        iterILS = 0;

        while (iterILS < Iils)
        {
            // s <-- rvnd(s)
            s = rvnd(s, c);

            if (f(s, c) < f(s_, c))
            {
                s_ = s;
                iterILS = 0;
            }

            // s <-- pertub(s')
            s = pertub(s_);
            iterILS++;
        }

        double aux = f(s_, c); // para reduzir a quatidade de vezes que f e chamada
        if (aux < f_)
        {
            _s_ = s_;
            f_ = aux;
        }
    }
    return _s_;
}

std::vector<int> rvnd(std::vector<int> s, double **c)
{
    std::vector<int> s_;

    // Inicializar estruturas de vizinhaca:
    // 1 - 2-opt
    // 2 - swap
    // 3 - sub_reinsertion
    std::vector<int> NL = {1, 2, 3};

    while (NL.size() > 0)
    {

        // Escolher randomicamente uma estrutura NL_n em NL
        int NL_n = *getRandonEle(NL.begin(), NL.end());

        double f_s = f(s, c); // para reduzir a quatidade de vezes que f e chamada

        switch (NL_n)
        {
        case 1:
            s_ = two_opt(s, f_s, c);
            break;
        case 2:
            s_ = swap(s, f_s, c);
            break;
        case 3:
            s_ = sub_reinsertion(s, f_s, c);
            break;
        default:
            break;
        }

        double _f_s = f(s_, c);

        if (_f_s < f_s)
        {
            s = s_;
            f_s = _f_s;
        }
        else
        {
            // Remover o valor NL_n de NL
            eraseByValue2(NL, NL_n);
        }
    }
    return s;
}

std::vector<int> pertub(std::vector<int> s)
{
    int N = s.size();

    // Utilizando shuffle para gerar os distrubios em s
    auto rng = std::default_random_engine{};
    std::shuffle(s.begin() + 1, s.end() - 1, rng);

    return s;
};

void eraseByValue2(std::vector<int> &vec, int val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

double f(std::vector<int> &s, double **c)
{
    double val = 0;
    for (int i = 0, j = 1; i < s.size() - 1; i++, j++)
    {
        val += c[s[i]][s[j]];
    }
    return val;
}