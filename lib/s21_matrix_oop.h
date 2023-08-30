#ifndef S21_MATRIX_OOP_LIB_S21_MATRIX_OOP_H_
#define S21_MATRIX_OOP_LIB_S21_MATRIX_OOP_H_

#include <cmath>

#include <iostream>

const double kEpsilon = 1.0E-8;

class S21Matrix {
 public:
  S21Matrix() noexcept;               // Default constructor
  S21Matrix(int rows_, int cols_);    // My constructor
  S21Matrix(const S21Matrix &other);  // Copy
  S21Matrix(S21Matrix &&other);       // Move
  ~S21Matrix();                       // Destructor

  int GetRows() const noexcept;
  int GetCols() const noexcept;
  void SetRows(const int rows);
  void SetCols(const int cols);

  bool EqMatrix(const S21Matrix &other) const;
  void SumMatrix(const S21Matrix &other);
  void SubMatrix(const S21Matrix &other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix &other);
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  bool operator==(const S21Matrix &other) const noexcept;
  bool operator!=(const S21Matrix &other) const noexcept;
  S21Matrix &operator=(const S21Matrix &other);
  S21Matrix &operator=(S21Matrix &&other);
  S21Matrix operator+(const S21Matrix &second) const;
  S21Matrix operator-(const S21Matrix &second) const;
  S21Matrix operator*(int number) const;
  S21Matrix operator*(const S21Matrix &other) const;
  const S21Matrix &operator+=(const S21Matrix &other);
  const S21Matrix &operator-=(const S21Matrix &other);
  const S21Matrix &operator*=(const S21Matrix &other);
  const S21Matrix &operator*=(int number) noexcept;
  double &operator()(int i, int j);
  double const &operator()(int i, int j) const;

 private:
  int rows_, cols_;
  double **matrix_;

  void InitializeMatrix() noexcept;
  bool EqualSize(const S21Matrix &other) const noexcept;
  bool EqualNumbers(const S21Matrix &other) const noexcept;
  bool EqualForMult(const S21Matrix &other) const noexcept;
  bool SquareMatrix() const noexcept;
  void SwapMatrix(S21Matrix &other);
  void MatrixMinors(const S21Matrix &other) const;
  void ShiftMatrix(const S21Matrix &other, int row_not,
                   int colum_not) const noexcept;
};

#endif  // S21_MATRIX_OOP_LIB_S21_MATRIX_OOP_H_
