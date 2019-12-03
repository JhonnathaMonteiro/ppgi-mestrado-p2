// A Dynamic Programming based solution for 0-1 Knapsack problem
#include <stdio.h>
#include <iostream>
#include "knapsack_solver.h"

// A utility function that returns maximum of two integers
int _max(int a, int b) { return (a > b) ? a : b; }

std::vector<double> _disclosure_content(std::vector<std::vector<int>> &contentMatrix,
                                        Matrix<double> &W)
{
  unsigned n = contentMatrix.size() - 1;
  unsigned capacidade = contentMatrix[0].size() - 1;
  unsigned k = capacidade;
  std::vector<double> content(n);

  for (unsigned i = n; i != 0; i--)
  {
    if (contentMatrix[i][k] == 1)
    {
      content[i - 1] = 1;
      k = capacidade - W[0][i - 1];
    }
  }
  return content;
}

// Algoritmo para resolucao do knapsack-0-1 Pseudo Polinomial O(nm)
Matrix<double> knapsack_solver(Matrix<double> &W,
                               Matrix<double> &PI,
                               double c)
{
  int n = (int)W.get_cols();
  int i, w;
  std::vector<std::vector<int>> K(n + 1, std::vector<int>(c + 1));
  std::vector<std::vector<int>> contentMatrix(n + 1, std::vector<int>(c + 1));

  // Build table K[][] in bottom up manner
  for (i = 1; i != n + 1; i++)
  {
    for (w = 0; w != c + 1; w++)
    {
      if (w - W[0][i - 1] >= 0)
      {
        if (K[i - 1][w] < (K[i - 1][w - W[0][i - 1]] + PI[0][i - 1]))
        {
          K[i][w] = _max(PI[0][i - 1] + K[i - 1][w - W[0][i - 1]], K[i - 1][w]);
          contentMatrix[i][w] = 1;
        }
        else
        {
          K[i][w] = K[i - 1][w];
        }
      }
    }
  }

  Matrix<double> result(_disclosure_content(contentMatrix, W));
  result = result.transpose();
  return result;
}
