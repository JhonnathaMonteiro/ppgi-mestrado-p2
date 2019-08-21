#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <algorithm>
#include <random>

/**
 * Escolher randomicamente (uniform distribution) um elemento em um vetor
 *
 */
template <typename Iter, typename RandomGenerator>
Iter getRandonEle(Iter start, Iter end, RandomGenerator &g)
{
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template <typename Iter>
Iter getRandonEle(Iter start, Iter end)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return getRandonEle(start, end, gen);
}

/**
 * Gerar valores randomicamente (uniform distribution)
 * no intervalo fechado [init, end]
 * engine mt19937 default
 *
 * @param init vetor
 * @param end valor
 */
template <typename Rtype>
Rtype getRandInt(Rtype init, Rtype end)
{
    std::random_device rseed;
    std::mt19937 rgen(rseed());

    std::uniform_int_distribution<Rtype> dist(init, end);

    return dist(rgen);
}

/**
 * Remover elemento por valor em um determinado vetor
 *
 * @param vec vetor
 * @param val valor
 */
template <typename My_type>
void eraseByValue(std::vector<My_type> &vec, My_type val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

#endif // UTILS_H_INCLUDED