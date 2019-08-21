#ifndef MOVEMENTS_H_INCLUDED
#define MOVEMENTS_H_INCLUDED
#include <vector>
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <time.h>
#include <random>

std::vector<int> two_opt(std::vector<int>, double, double **);
std::vector<int> swap(std::vector<int>, double, double **);
std::vector<int> sub_reinsertion(std::vector<int>, double, double **);
#endif // MOVEMENTS_H_INCLUDED
