#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <vector>
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <random>
#include <time.h>
#include <limits>

template <typename Iter>
Iter getRandonEle(Iter, Iter);

template <typename Rtype>
Rtype uniRand(Rtype, Rtype);

template <typename My_type>
void eraseByValue(std::vector<My_type> &, My_type);

#endif // UTILS_H_INCLUDED