// A Dynamic Programming based solution for 0-1 Knapsack problem
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "knapsack_solver.h"

Matrix<double> kp_solver(Matrix<double> &W,
                         Matrix<double> &PI, int c)
{
  int n = (int)W.get_cols();
  Matrix<double> s(n, 1, 0);
  // gerando o conjunto de itens
  std::vector<item_t> items(n);
  for (int i = 0; i < n; i++)
  {
    item_t item = {(int)W[0][i], PI[0][i]};
    items[i] = item;
  }

  // programacao dinamica
  int i, j, a, b, *mm, **m;
  mm = (int *)calloc((n + 1) * (c + 1), sizeof(int));
  m = (int **)malloc((n + 1) * sizeof(int *));
  m[0] = mm;
  for (i = 1; i <= n; i++)
  {
    m[i] = &mm[i * (c + 1)];
    for (j = 0; j <= c; j++)
    {
      if (items[i - 1].weight > j)
      {
        m[i][j] = m[i - 1][j];
      }
      else
      {
        a = m[i - 1][j];
        b = m[i - 1][j - items[i - 1].weight] + items[i - 1].value;
        m[i][j] = a > b ? a : b;
      }
    }
  }
  for (i = n, j = c; i > 0; i--)
  {
    if (m[i][j] > m[i - 1][j])
    {
      s[i - 1][0] = 1;
      j -= items[i - 1].weight;
    }
  }

  // liberando memoria
  free(mm);
  free(m);
  return s;
}
