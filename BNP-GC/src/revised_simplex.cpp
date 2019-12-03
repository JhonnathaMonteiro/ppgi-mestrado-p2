#include <limits>
#include <iostream>

#include "revised_simplex.h"
#include "matrix.h"
#include "knapsack_solver.h"

RV rv_simplex(Matrix<double> W,
              Matrix<double> b,
              Matrix<double> C,
              Matrix<double> B,
              double c, double tol_gc, unsigned n)
{
  B[3][0] = 1;
  B[2][1] = 1;
  B[3][2] = 1;
  // TODO: Um refinamento pode ser feito na BSI para diminuir a quantidade
  //       de colunas geradas.

  // Declaracoes
  Matrix<double> B_inv = B.inverse(); // Calculo da inversa (uma vez por no)
  Matrix<double> Xe, _Xe, X_B, PI;
  double r, OPT, min_ratio, pivot;
  unsigned l = 0; // min ratio index
  RV result;
  int counter = 1;

  while (true)
  {
    X_B = B_inv * b;
    PI = C.transpose() * B_inv;
    OPT = (PI * b)[0][0];

    // Encontrando a coluna que vai entrar na base
    // Resolver o KnapSack 0-1: Max PI*Xe; st. W*Xe <= c, Xe e {0,1}
    // Encontrar Xe
    Xe = knapsack_solver(W, PI, c);

    r = (PI * Xe)[0][0] - 1; // Custo reduzido

    if (r <= tol_gc)
    {
      // Optimalidade provada
      break;
    }
    else
    {
      _Xe = B_inv * Xe;
      // Teste da razao
      min_ratio = std::numeric_limits<double>::infinity();
      for (unsigned i = 0; i < n; i++)
      {
        if (_Xe[i][0] > 0)
        {
          if ((X_B[i][0] / _Xe[i][0]) < min_ratio)
          {
            min_ratio = X_B[i][0] / _Xe[i][0];
            l = i;
          }
        }
      }

      if (min_ratio == std::numeric_limits<double>::infinity())
      {
        // teste da razao falhou!
        // problema unbounded
        // TODO: Tratar unbounded aqui
        std::cout << "UNBOUND" << std::endl;
        break;
      }

      // TESTE
      std::cout << "GC: " << counter << std::endl;
      ++counter;

      pivot = _Xe[l][0];
      Matrix<double> A(n, "EYE"); //Matriz identidade
      for (unsigned i = 0; i < n; i++)
      {
        B[i][l] = Xe[i][0]; // mudando a base
        if (i == l)
        {
          A[i][l] = 1 / pivot;
          continue;
        }
        if (_Xe[i][0] == 0)
          continue;

        A[i][l] = -_Xe[i][0] / pivot;
      }
      B_inv = A * B_inv;
    }
  }
  result.B = B;
  result.OPTCG = OPT;
  result.X_B = X_B;
  return result;
}