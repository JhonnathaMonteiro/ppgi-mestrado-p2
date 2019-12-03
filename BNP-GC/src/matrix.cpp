#ifndef MATRIX_CPP
#define MATRIX_CPP

#include "matrix.h"

// Construtor por parametros
template <typename T>
Matrix<T>::Matrix(unsigned _rows, unsigned _cols, const T &_initial)
{
  mat.resize(_rows);
  for (unsigned i = 0; i < mat.size(); i++)
  {
    mat[i].resize(_cols, _initial);
  }
  rows = _rows;
  cols = _cols;
}

// Construtor para tipos especificos
template <typename T>
Matrix<T>::Matrix(unsigned _size, std::string _m_type)
{
  mat.resize(_size);

  if (_m_type == "EYE")
  {
    for (unsigned i = 0; i < mat.size(); i++)
    {
      mat[i].resize(_size, 0);
    }
    for (unsigned i = 0; i < mat.size(); i++)
    {
      mat[i][i] = 1;
    }
  }

  cols = rows = _size;
}

// Construtor por copia
template <typename T>
Matrix<T>::Matrix(const Matrix<T> &rhs)
{
  mat = rhs.mat;
  rows = rhs.get_rows();
  cols = rhs.get_cols();
}

// Construtor por copia de vetor
template <typename T>
Matrix<T>::Matrix(const std::vector<T> &rhs)
{
  mat.push_back(rhs);
  rows = 1;
  cols = (unsigned)rhs.size();
}

// (Virtual) Destructor
template <typename T>
Matrix<T>::~Matrix() {}

// Operador de Assignment
template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &rhs)
{
  if (&rhs == this)
    return *this;

  unsigned new_rows = rhs.get_rows();
  unsigned new_cols = rhs.get_cols();

  mat.resize(new_rows);
  for (unsigned i = 0; i < mat.size(); i++)
  {
    mat[i].resize(new_cols);
  }

  for (unsigned i = 0; i < new_rows; i++)
  {
    for (unsigned j = 0; j < new_cols; j++)
    {
      mat[i][j] = rhs(i, j);
    }
  }
  rows = new_rows;
  cols = new_cols;

  return *this;
}

// Adicao de duas matrizes
template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result(i, j) = this->mat[i][j] + rhs(i, j);
    }
  }

  return result;
}

// Adicao cumulativa
template <typename T>
Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &rhs)
{
  unsigned rows = rhs.get_rows();
  unsigned cols = rhs.get_cols();

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      this->mat[i][j] += rhs(i, j);
    }
  }

  return *this;
}

// Subtracao de matrizes
template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &rhs)
{
  unsigned rows = rhs.get_rows();
  unsigned cols = rhs.get_cols();
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result(i, j) = this->mat[i][j] - rhs(i, j);
    }
  }

  return result;
}

// Subtracao cumulativa
template <typename T>
Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &rhs)
{
  unsigned rows = rhs.get_rows();
  unsigned cols = rhs.get_cols();

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      this->mat[i][j] -= rhs(i, j);
    }
  }

  return *this;
}

// Multplicacao esquerda dessa matriz por outra
template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &rhs)
{
  unsigned rows = this->get_rows();
  unsigned cols = rhs.get_cols();
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      for (unsigned k = 0; k < rhs.get_rows(); k++)
      {
        result(i, j) += this->mat[i][k] * rhs(k, j);
      }
    }
  }

  return result;
}

// Multplicacao cumulativa esquerda dessa matriz por outra
template <typename T>
Matrix<T> &Matrix<T>::operator*=(const Matrix<T> &rhs)
{
  Matrix result = (*this) * rhs;
  (*this) = result;
  return *this;
}

// Transposta
template <typename T>
Matrix<T> Matrix<T>::transpose()
{
  Matrix result(cols, rows, 0.0);
  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result(j, i) = this->mat[i][j];
    }
  }

  return result;
}

// Inversa
template <typename T>
Matrix<T> Matrix<T>::inverse()
{
  Matrix result(rows, cols, 0.0);

  // Decomposicao LU
  int n = (int)mat.size();
  std::vector<std::vector<double>> L(n, std::vector<double>(n, 0));
  std::vector<std::vector<double>> U(n, std::vector<double>(n, 0));

  for (int i = 0; i < n; i++)
  {

    // Triangular superior
    for (int k = i; k < n; k++)
    {

      // Soma L(i, j) * U(j, k)
      int sum = 0;
      for (int j = 0; j < i; j++)
        sum += (L[i][j] * U[j][k]);

      // Eval U(i, k)
      U[i][k] = mat[i][k] - sum;
    }

    // Triangular iferior
    for (int k = i; k < n; k++)
    {
      if (i == k)
        L[i][i] = 1; // Diagonal como sendo 1
      else
      {

        // Soma de L(k, j) * U(j, i)
        int sum = 0;
        for (int j = 0; j < i; j++)
          sum += (L[k][j] * U[j][i]);

        // Avaliando L(k, i)
        L[k][i] = (mat[k][i] - sum) / U[i][i];
      }
    }
  }

  // calculando inverca
  std::vector<std::vector<double>> I(n, std::vector<double>(n, 0));
  std::vector<std::vector<double>> D(n, std::vector<double>(n, 0));
  for (int i = 0; i < n; i++)
  {
    I[i][i] = I[i][i] = 1;
    D[i][i] = 1 / mat[i][i];
  }
  double tmp = 0;
  for (int i = 0; i < n; i++)
  {
    for (int j = 1; j < n; j++)
    {
      for (int k = 0; k <= j - 1; k++)
      {
        tmp += mat[j][k] * D[k][i];
      }
      D[j][i] = (I[j][i] - tmp) / mat[j][j];
      tmp = 0;
    }
  }

  for (int i = 0; i < n; i++)
  {
    result.mat[n - 1][i] = D[n - 1][i];
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = n - 2; j >= 0; j--)
    {
      for (int k = n - 1; k > j; k--)
      {
        tmp += U[j][k] * result.mat[k][i];
      }
      result.mat[j][i] = D[j][i] - tmp;
      tmp = 0;
    }
  }
  return result;
}

// Cofator
template <typename T>
Matrix<T> Matrix<T>::cofactor(Matrix<T> &A, unsigned p, unsigned q, unsigned n)
{
  unsigned i = 0, j = 0;
  Matrix result(n, n, 0.0);
  for (unsigned row = 0; row < n; row++)
  {
    for (unsigned col = 0; col < n; col++)
    {

      if (row != p && col != q)
      {
        result[i][j++] = A[row][col];
        if (j == n - 1)
        {
          j = 0;
          i++;
        }
      }
    }
  }
  return result;
}

// Determinante
template <typename T>
T Matrix<T>::determinant(Matrix<T> &A, unsigned n)
{
  // Tecnica recursiva para o calculo do determinante
  T result = 0;
  if (n == 1)
    return A[0][0];

  int sign = 1;
  for (unsigned f = 0; f < n; f++)
  {
    Matrix temp = cofactor(A, 0, f, n);
    result += sign * A[0][f] * determinant(temp, n - 1);
    sign = -sign;
  }

  return result;
}

// Matrix/scalar addicao
template <typename T>
Matrix<T> Matrix<T>::operator+(const T &rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result(i, j) = this->mat[i][j] + rhs;
    }
  }

  return result;
}

// Matrix/scalar subtracao
template <typename T>
Matrix<T> Matrix<T>::operator-(const T &rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result(i, j) = this->mat[i][j] - rhs;
    }
  }

  return result;
}

// Matrix/scalar multiplicacao
template <typename T>
Matrix<T> Matrix<T>::operator*(const T &rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result(i, j) = this->mat[i][j] * rhs;
    }
  }

  return result;
}

// Matrix/scalar divisao
template <typename T>
Matrix<T> Matrix<T>::operator/(const T &rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result(i, j) = this->mat[i][j] / rhs;
    }
  }

  return result;
}

// Multiplicacao uma matriz com um vetor
template <typename T>
std::vector<T> Matrix<T>::operator*(const std::vector<T> &rhs)
{
  std::vector<T> result(rhs.size(), 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result[i] = this->mat[i][j] * rhs[j];
    }
  }

  return result;
}

// Vetor da diagonal principal
template <typename T>
std::vector<T> Matrix<T>::diag_vec()
{
  std::vector<T> result(rows, 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    result[i] = this->mat[i][i];
  }

  return result;
}

// Acesso a elemento individual
template <typename T>
T &Matrix<T>::operator()(const unsigned &row, const unsigned &col)
{
  return this->mat[row][col];
}

template <typename T>
std::vector<T> &Matrix<T>::operator[](const unsigned &item)
{
  return this->mat[item];
}

// Acesso a elemento individual (const)
template <typename T>
const T &Matrix<T>::operator()(const unsigned &row, const unsigned &col) const
{
  return this->mat[row][col];
}

// Numero de linhas da matriz
template <typename T>
unsigned Matrix<T>::get_rows() const
{
  return this->rows;
}

// Numero de colunas da matriz
template <typename T>
unsigned Matrix<T>::get_cols() const
{
  return this->cols;
}

#endif