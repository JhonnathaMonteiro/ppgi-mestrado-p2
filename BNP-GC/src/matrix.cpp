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
  unsigned rows = rhs.get_rows();
  unsigned cols = rhs.get_cols();
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      for (unsigned k = 0; k < rows; k++)
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
  Matrix result(rows, cols, 0.0);
  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result(i, j) = this->mat[j][i];
    }
  }

  return result;
}

// Inversa
template <typename T>
Matrix<T> Matrix<T>::inverse()
{
  Matrix result(rows, cols, 0.0);
  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      result(i, j) = this->mat[i][j];
    }
  }

  // CALCULAR A INVERSA EM result AQUI

  return result;
}

// Adjunta
template <typename T>
Matrix<T> Matrix<T>::adjoint()
{
  Matrix result(rows, cols, 0.0);
  if (mat.size() == 1)
  {
    result[0][0] = 1;
    return result;
  }

  int sign = 1;
  for (unsigned row = 0; row < this->get_rows(); row++)
  {
    for (unsigned col = 0; col < this->get_cols(); col++)
    {
      Matrix<T> temp = cofactor(*this, row, col, this->get_rows());
      sign = ((row + col) % 2 == 0) ? 1 : -1;

      result[col][row] = (sign) * (determinant(temp, this->get_rows() - 1));
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
    return this->mat[0][0];

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