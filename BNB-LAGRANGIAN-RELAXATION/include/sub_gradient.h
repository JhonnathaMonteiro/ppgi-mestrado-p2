#ifndef SUB_GRADIENT_H
#define SUB_GRADIENT_H

#include <cstdio>
#include <vector>
#include <utility>
#include <limits>
#include <algorithm> // std::fill
#include <iterator>  // std::distance
#include <math.h>    //pow

void subgrad_method(Node &, double **, int, std::vector<double>, double, double);

#endif