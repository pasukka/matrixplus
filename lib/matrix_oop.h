#ifndef _MATRIX_OOP_LIB__MATRIX_OOP_H_
#define _MATRIX_OOP_LIB__MATRIX_OOP_H_

#include <cmath>
#include <iostream>

const double kEpsilon = 1.0E-8;

class Matrix {
 public:
  Matrix() noexcept;             // Default constructor
  Matrix(int rows_, int cols_);  // My constructor
  ~Matrix();                     // Destructor

  Matrix(const Matrix &other);  // Copy
  Matrix(Matrix &&other);       // Move

  int GetRows() const noexcept;
  int GetCols() const noexcept;
  void SetRows(const int rows);
  void SetCols(const int cols);

  bool EqMatrix(const Matrix &other) const;
  void SumMatrix(const Matrix &other);
  void SubMatrix(const Matrix &other);
  void MulNumber(const double num);
  void MulMatrix(const Matrix &other);
  Matrix Transpose() const;
  Matrix CalcComplements() const;
  double Determinant() const;
  Matrix InverseMatrix() const;

  bool operator==(const Matrix &other) const noexcept;
  bool operator!=(const Matrix &other) const noexcept;
  Matrix &operator=(const Matrix &other);
  Matrix &operator=(Matrix &&other);

  Matrix operator+(const Matrix &second) const;
  Matrix operator-(const Matrix &second) const;
  Matrix operator*(int number) const;
  Matrix operator*(const Matrix &other) const;
  friend const Matrix operator*(int number, const Matrix &matrix);

  const Matrix &operator+=(const Matrix &other);
  const Matrix &operator-=(const Matrix &other);
  const Matrix &operator*=(const Matrix &other);
  const Matrix &operator*=(int number) noexcept;

  double &operator()(int i, int j);
  double const &operator()(int i, int j) const;

 private:
  int rows_, cols_;
  double **matrix_;

  void InitializeMatrix() noexcept;
  bool EqualSize(const Matrix &other) const noexcept;
  bool EqualNumbers(const Matrix &other) const noexcept;
  bool EqualForMult(const Matrix &other) const noexcept;
  bool SquareMatrix() const noexcept;
  void SwapMatrix(Matrix &other);
  void MatrixMinors(const Matrix &other) const;
  void ShiftMatrix(const Matrix &other, int row_not,
                   int colum_not) const noexcept;
};

#endif  // _MATRIX_OOP_LIB__MATRIX_OOP_H_

const Matrix operator*(int number, const Matrix &matrix);