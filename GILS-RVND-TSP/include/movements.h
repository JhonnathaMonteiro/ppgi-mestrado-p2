#ifndef MOVEMENTS_H_INCLUDED
#define MOVEMENTS_H_INCLUDED
#include <vector>

std::vector<int> two_opt(std::vector<int>, double, double **);
std::vector<int> swap(std::vector<int>, double, double **);
// std::vector<int> sub_reinsertion(std::vector<int>, double, double **);
std::vector<int> reinsertion(std::vector<int>, double, double **);
std::vector<int> or_2opt(std::vector<int>, double, double **);
std::vector<int> or_3opt(std::vector<int>, double, double **);
#endif // MOVEMENTS_H_INCLUDED
