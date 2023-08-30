#include "matrix_oop.h"

// --------------------- CREATION AND DESTRUCTION ---------------------

Matrix::Matrix(int rows_, int cols_)
    : rows_(rows_), cols_(cols_), matrix_(nullptr) {
  matrix_ = (double**)new double*[rows_];
  for (int i = 0; i < rows_; ++i) {
    try {
      matrix_[i] = (double*)new double[cols_];
    } catch (...) {
      for (int j = 0; j < i; ++j) delete[] matrix_[j];
      delete[] matrix_;
      throw;
    }
  }
  InitializeMatrix();
}

Matrix::Matrix() noexcept : rows_(0), cols_(0), matrix_(nullptr) {}

Matrix::~Matrix() {
  for (int i = 0; i < rows_; ++i) {
    delete[] matrix_[i];
  }
  if (rows_ > 0) delete[] matrix_;
}

// --------------------- COPY AND MOVE ---------------------
Matrix::Matrix(const Matrix& other) : rows_(0), cols_(0), matrix_(nullptr) {
  if (other.rows_ > 0 && other.cols_ > 0) {
    Matrix result(other.rows_, other.cols_);
    for (int i = 0; i < other.rows_; ++i) {
      for (int j = 0; j < other.cols_; ++j) {
        result.matrix_[i][j] = other.matrix_[i][j];
      }
    }
    SwapMatrix(result);
  }
}

Matrix::Matrix(Matrix&& other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

// --------------------- ACCESSORS AND MUTATORS ---------------------

int Matrix::GetRows() const noexcept { return rows_; }

int Matrix::GetCols() const noexcept { return cols_; }

void Matrix::SetRows(const int new_rows) {
  if (new_rows < 0) {
    throw std::invalid_argument("Number of rows less than 0.");
  }
  if (rows_ < new_rows) {
    Matrix result(new_rows, cols_);
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        result.matrix_[i][j] = matrix_[i][j];
      }
    }
    SwapMatrix(result);
  } else {
    for (int i = new_rows; i < rows_; ++i) {
      delete[] matrix_[i];
    }
    rows_ = new_rows;
  }
}

void Matrix::SetCols(const int new_cols) {
  if (new_cols < 0) {
    throw std::invalid_argument("Number of columns less than 0.");
  }
  Matrix result(rows_, new_cols);
  int cols = (cols_ < new_cols) ? cols_ : new_cols;
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols; ++j) {
      result.matrix_[i][j] = matrix_[i][j];
    }
  }
  SwapMatrix(result);
}

// --------------------- OPERATIONS ---------------------
bool Matrix::EqMatrix(const Matrix& other) const { return *this == other; }

void Matrix::SumMatrix(const Matrix& other) { *this += other; }

void Matrix::SubMatrix(const Matrix& other) { *this -= other; }

void Matrix::MulNumber(const double num) { *this *= num; }

void Matrix::MulMatrix(const Matrix& other) { *this *= other; }

Matrix Matrix::Transpose() const {
  Matrix result(cols_, rows_);
  for (int i = 0; i < cols_; ++i) {
    for (int j = 0; j < rows_; ++j) {
      result.matrix_[i][j] = matrix_[j][i];
    }
  }
  return result;
}

Matrix Matrix::CalcComplements() const {
  if (rows_ == cols_ && cols_ == 0) {
    throw std::invalid_argument("The matrix is not initialised.");
  }
  if (!SquareMatrix()) {
    throw std::invalid_argument("The matrix is not square.");
  }
  Matrix result(rows_, cols_);
  MatrixMinors(result);
  for (int i = 0; i < result.rows_; ++i) {
    for (int j = 0; j < result.cols_; ++j) {
      result.matrix_[i][j] *= pow(-1, i + j);
    }
  }
  return result;
}

double Matrix::Determinant() const {
  if (!SquareMatrix()) {
    throw std::invalid_argument("The matrix is not square.");
  }
  double result = 0;
  Matrix matrix(rows_, cols_);
  switch (rows_) {
    case 1:
      result = matrix_[0][0];
      break;
    case 2:
      result = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
      break;
    default:
      double res = 0;
      for (int i = 0; i < rows_; ++i) {
        matrix = CalcComplements();
        res += matrix_[0][i] * matrix.matrix_[0][i];
      }
      result = res;
  }
  return result;
}

Matrix Matrix::InverseMatrix() const {
  double determinant = Determinant();
  if (fabs(determinant) < kEpsilon) {
    throw std::invalid_argument("The matrix determinant is 0.");
  }
  Matrix result(rows_, cols_);
  if (rows_ == 1) {
    result.matrix_[0][0] = 1 / matrix_[0][0];
  } else {
    result = CalcComplements();
    result = result.Transpose();
    for (int i = 0; i < result.rows_; ++i) {
      for (int j = 0; j < result.cols_; ++j) {
        result.matrix_[i][j] /= determinant;
      }
    }
  }
  return result;
}

// --------------------- OPERATORS ---------------------
Matrix& Matrix::operator=(const Matrix& other) {
  Matrix copy(other);
  SwapMatrix(copy);
  return *this;
}

Matrix& Matrix::operator=(Matrix&& other) {
  SwapMatrix(other);
  return *this;
}

bool Matrix::operator==(const Matrix& other) const noexcept {
  bool output = false;
  if (EqualSize(other)) {
    output = EqualNumbers(other);
  }
  return output;
}

bool Matrix::operator!=(const Matrix& other) const noexcept {
  return !(*this == other);
}

Matrix Matrix::operator+(const Matrix& second) const {
  Matrix result = *this;
  result += second;
  return result;
}

Matrix Matrix::operator-(const Matrix& second) const {
  Matrix result = *this;
  result -= second;
  return result;
}

Matrix Matrix::operator*(int number) const {
  Matrix result = *this;
  result *= number;
  return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
  if (!EqualForMult(other)) {
    throw std::invalid_argument(
        "The number of columns of the first matrix is not equal to the number "
        "of rows of the second matrix.");
  }
  Matrix result(rows_, other.cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < other.cols_; ++j) {
      for (int k = 0; k < cols_; ++k) {
        result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  return result;
}

const Matrix operator*(int number, const Matrix& matrix) {
  return matrix * number;
}

const Matrix& Matrix::operator+=(const Matrix& other) {
  if (!EqualSize(other)) {
    throw std::invalid_argument("Different matrix dimensions.");
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j];
    }
  }
  return *this;
}

const Matrix& Matrix::operator-=(const Matrix& other) {
  if (!EqualSize(other)) {
    throw std::invalid_argument("Different matrix dimensions.");
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = matrix_[i][j] - other.matrix_[i][j];
    }
  }
  return *this;
}

const Matrix& Matrix::operator*=(int number) noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = matrix_[i][j] * number;
    }
  }
  return *this;
}

const Matrix& Matrix::operator*=(const Matrix& other) {
  *this = *this * other;
  return *this;
}

double& Matrix::operator()(int i, int j) {
  if (i >= rows_ && j >= cols_) {
    throw std::out_of_range("Index out of range.");
  }
  return matrix_[i][j];
}

const double& Matrix::operator()(int i, int j) const {
  if (i >= rows_ && j >= cols_) {
    throw std::out_of_range("Index out of range.");
  }
  return matrix_[i][j];
}

// --------------------- UTILS ---------------------

void Matrix::InitializeMatrix() noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = 0;
    }
  }
}

void Matrix::SwapMatrix(Matrix& other) {
  std::swap(matrix_, other.matrix_);
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
}

bool Matrix::EqualSize(const Matrix& other) const noexcept {
  return (rows_ == other.rows_ && cols_ == other.cols_);
}

bool Matrix::SquareMatrix() const noexcept { return (rows_ == cols_); }

bool Matrix::EqualForMult(const Matrix& other) const noexcept {
  return (cols_ == other.rows_);
}

bool Matrix::EqualNumbers(const Matrix& other) const noexcept {
  bool output = true;
  for (int i = 0; i < rows_ && output; ++i) {
    for (int j = 0; j < cols_ && output; ++j) {
      if (fabs(this->matrix_[i][j] - other.matrix_[i][j]) > kEpsilon) {
        output = false;
      }
    }
  }
  return output;
}

void Matrix::MatrixMinors(const Matrix& other) const {
  double minor;
  if (rows_ == 1) {
    minor = Determinant();
    other.matrix_[0][0] = minor;
  } else {
    Matrix for_minor(rows_ - 1, cols_ - 1);
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        ShiftMatrix(for_minor, i, j);
        minor = for_minor.Determinant();
        other.matrix_[i][j] = minor;
      }
    }
  }
}

void Matrix::ShiftMatrix(const Matrix& other, int row_not,
                         int colum_not) const noexcept {
  int shift_row = 0;
  for (int row = 0; row < other.rows_; ++row) {
    if (row == row_not) shift_row = 1;
    int shift_column = 0;
    for (int column = 0; column < other.rows_; ++column) {
      if (column == colum_not) shift_column = 1;
      other.matrix_[row][column] =
          matrix_[row + shift_row][column + shift_column];
    }
  }
}
