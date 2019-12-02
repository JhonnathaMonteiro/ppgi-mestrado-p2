#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>

// Tipos de matrizes (nxn para o construtor)
#define ZERO 0
#define EYE 1

template <typename T>
class Matrix
{
private:
  std::vector<std::vector<T>> mat;
  unsigned rows;
  unsigned cols;

public:
  Matrix(unsigned _rows, unsigned _cols, const T &_initial);
  Matrix(unsigned _size, std::string _m_type);
  Matrix(const Matrix<T> &rhs);      // Construtor por copia
  Matrix(const std::vector<T> &rhs); // Construtor por copia de vetor
  virtual ~Matrix();

  // Sobrecarga de operadores para operacoes basicas
  Matrix<T> &operator=(const Matrix<T> &rhs);
  Matrix<T> operator+(const Matrix<T> &rhs);
  Matrix<T> &operator+=(const Matrix<T> &rhs);
  Matrix<T> operator-(const Matrix<T> &rhs);
  Matrix<T> &operator-=(const Matrix<T> &rhs);
  Matrix<T> operator*(const Matrix<T> &rhs);
  Matrix<T> &operator*=(const Matrix<T> &rhs);

  Matrix<T> transpose();
  Matrix<T> adjoint();
  Matrix<T> inverse();
  Matrix<T> cofactor(Matrix<T> &A, unsigned p, unsigned q, unsigned n);

  T determinant(Matrix<T> &A, unsigned n);

  // Operacoes entre Matriz e escalar
  Matrix<T> operator+(const T &rhs);
  Matrix<T> operator-(const T &rhs);
  Matrix<T> operator*(const T &rhs);
  Matrix<T> operator/(const T &rhs);

  // Matrix/vector operations
  std::vector<T> operator*(const std::vector<T> &rhs);
  std::vector<T> diag_vec();

  // Operadores de acesso
  T &operator()(const unsigned &row, const unsigned &col);
  const T &operator()(const unsigned &row, const unsigned &col) const;
  std::vector<T> &operator[](const unsigned &item);

  // Tamanho das colunas de linhas
  unsigned get_rows() const;
  unsigned get_cols() const;
};

#include "../src/matrix.cpp"

#endif