#include <iostream>
#include "ffd_refinement.h"

// first fit decreasing algorithm - FFD para refinamento da solucao basica inicial
void _triangularUp(Matrix<double> &BFS, int k)
{
  // Function TriangularUp(BFS, k) returns all the
  // elements on and above the k th diagonal of the BFS.

  int n = (int)BFS.get_cols();

  for (int col = 0; col < n - k - 1; col++)
  {
    for (int row = k + col + 1; row < n; row++)
    {
      BFS[row][col] = 0;
    }
  }

  // for (int col = 0; col < n - k; col++)
  // {
  //   for (int row = k + 1; row < n; row++)
  //   {
  //     BFS[row][col] = 0;
  //   }
  // }
}

void ffd_refinament(Matrix<double> &BFS,
                    Matrix<double> &W,
                    Matrix<double> &b, double c)
{
  int n = (int)BFS.get_cols();
  Matrix<double> X_B;
  Matrix<double> BFS_j(n, 1, 0);

  for (int j = 0; j < n - 1; j++)
  {
    for (int i = j + 1; i < n; i++)
    {
      for (int k = 0; k < n; k++)
      {
        BFS_j[k][0] = BFS[k][j];
      }
      double aux = (W * BFS_j)[0][0] + W[0][i];
      if (aux <= c)
      {
        BFS[i][j] = 1;
      }
      else
      {
        BFS[i][j] = 0;
      }
    }
  }

  for (int k = n - 1; k >= 0; k--)
  {
    _triangularUp(BFS, k);

    X_B = BFS.inverse() * b;

    int negatives = 0;
    for (int i = 0; i < n; i++)
    {
      if (X_B[i][0] < 0)
      {
        negatives += 1;
      }
    }
    if (negatives == 0)
    {
      break;
    }
  }
}