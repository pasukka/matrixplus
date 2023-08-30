#include "s21_matrix_oop.h"

// --------------------- CREATION AND DESTRUCTION ---------------------

S21Matrix::S21Matrix(int rows_, int cols_)
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

S21Matrix::S21Matrix() noexcept : rows_(0), cols_(0), matrix_(nullptr) {}

S21Matrix::~S21Matrix() {
  for (int i = 0; i < rows_; ++i) {
    delete[] matrix_[i];
  }
  if (rows_ > 0) delete[] matrix_;
}

// --------------------- ACCESSORS AND MUTATORS ---------------------

int S21Matrix::GetRows() const noexcept { return rows_; }

int S21Matrix::GetCols() const noexcept { return cols_; }

void S21Matrix::SetRows(const int new_rows) {
  if (new_rows < 0) {
    throw std::invalid_argument("Number of rows less than 0.");
  }
  if (rows_ < new_rows) {
    S21Matrix result(new_rows, cols_);
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

void S21Matrix::SetCols(const int new_cols) {
  if (new_cols < 0) {
    throw std::invalid_argument("Number of columns less than 0.");
  }
  S21Matrix result(rows_, new_cols);
  int cols = (cols_ < new_cols) ? cols_ : new_cols;
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols; ++j) {
      result.matrix_[i][j] = matrix_[i][j];
    }
  }
  SwapMatrix(result);
}

// --------------------- COPY AND MOVE ---------------------
S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(0), cols_(0), matrix_(nullptr) {
  if (other.rows_ > 0 && other.cols_ > 0) {
    S21Matrix result(other.rows_, other.cols_);
    for (int i = 0; i < other.rows_; ++i) {
      for (int j = 0; j < other.cols_; ++j) {
        result.matrix_[i][j] = other.matrix_[i][j];
      }
    }
    SwapMatrix(result);
  }
}

S21Matrix::S21Matrix(S21Matrix&& other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

// --------------------- OPERATIONS ---------------------
bool S21Matrix::EqMatrix(const S21Matrix& other) const {
  return *this == other;
}

void S21Matrix::SumMatrix(const S21Matrix& other) { *this += other; }

void S21Matrix::SubMatrix(const S21Matrix& other) { *this -= other; }

void S21Matrix::MulNumber(const double num) { *this *= num; }

void S21Matrix::MulMatrix(const S21Matrix& other) { *this *= other; }

S21Matrix S21Matrix::Transpose() const {
  S21Matrix result(cols_, rows_);
  for (int i = 0; i < cols_; ++i) {
    for (int j = 0; j < rows_; ++j) {
      result.matrix_[i][j] = matrix_[j][i];
    }
  }
  return result;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ == cols_ && cols_ == 0) {
    throw std::invalid_argument("The matrix is not initialised.");
  }
  if (!SquareMatrix()) {
    throw std::invalid_argument("The matrix is not square.");
  }
  S21Matrix result(rows_, cols_);
  MatrixMinors(result);
  for (int i = 0; i < result.rows_; ++i) {
    for (int j = 0; j < result.cols_; ++j) {
      result.matrix_[i][j] *= pow(-1, i + j);
    }
  }
  return result;
}

double S21Matrix::Determinant() const {
  if (!SquareMatrix()) {
    throw std::invalid_argument("The matrix is not square.");
  }
  double result = 0;
  S21Matrix matrix(rows_, cols_);
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

S21Matrix S21Matrix::InverseMatrix() const {
  double determinant = Determinant();
  if (fabs(determinant) < EPSILON) {
    throw std::invalid_argument("The matrix determinant is 0.");
  }
  S21Matrix result(rows_, cols_);
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
S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  S21Matrix copy(other);
  SwapMatrix(copy);
  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) {
  SwapMatrix(other);
  return *this;
}

bool S21Matrix::operator==(const S21Matrix& other) const noexcept {
  bool output = false;
  if (EqualSize(other)) {
    output = EqualNumbers(other);
  }
  return output;
}

S21Matrix S21Matrix::operator+(const S21Matrix& second) const {
  S21Matrix result = *this;
  result += second;
  return result;
}

S21Matrix S21Matrix::operator-(const S21Matrix& second) const {
  S21Matrix result = *this;
  result -= second;
  return result;
}

S21Matrix S21Matrix::operator*(int number) const {
  S21Matrix result = *this;
  result *= number;
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  if (!EqualForMult(other)) {
    throw std::invalid_argument(
        "The number of columns of the first matrix is not equal to the number "
        "of rows of the second matrix.");
  }
  S21Matrix result(rows_, other.cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < other.cols_; ++j) {
      for (int k = 0; k < cols_; ++k) {
        result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  return result;
}

const S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
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

const S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
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

const S21Matrix& S21Matrix::operator*=(int number) noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = matrix_[i][j] * number;
    }
  }
  return *this;
}

const S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  *this = *this * other;
  return *this;
}

double& S21Matrix::operator()(int i, int j) {
  if (i >= rows_ && j >= cols_) {
    throw std::out_of_range("Index out of range.");
  }
  return matrix_[i][j];
}

const double& S21Matrix::operator()(int i, int j) const {
  if (i >= rows_ && j >= cols_) {
    throw std::out_of_range("Index out of range.");
  }
  return matrix_[i][j];
}

// --------------------- UTILS ---------------------

void S21Matrix::InitializeMatrix() noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = 0;
    }
  }
}

void S21Matrix::SwapMatrix(S21Matrix& other) {
  std::swap(matrix_, other.matrix_);
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
}

bool S21Matrix::EqualSize(const S21Matrix& other) const noexcept {
  return (rows_ == other.rows_ && cols_ == other.cols_);
}

bool S21Matrix::SquareMatrix() const noexcept { return (rows_ == cols_); }

bool S21Matrix::EqualForMult(const S21Matrix& other) const noexcept {
  return (cols_ == other.rows_);
}

bool S21Matrix::EqualNumbers(const S21Matrix& other) const noexcept {
  bool output = true;
  for (int i = 0; i < rows_ && output; ++i) {
    for (int j = 0; j < cols_ && output; ++j) {
      if (fabs(this->matrix_[i][j] - other.matrix_[i][j]) > EPSILON) {
        output = false;
      }
    }
  }
  return output;
}

void S21Matrix::MatrixMinors(const S21Matrix& other) const {
  double minor;
  if (rows_ == 1) {
    minor = Determinant();
    other.matrix_[0][0] = minor;
  } else {
    S21Matrix for_minor(rows_ - 1, cols_ - 1);
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        ShiftMatrix(for_minor, i, j);
        minor = for_minor.Determinant();
        other.matrix_[i][j] = minor;
      }
    }
  }
}

void S21Matrix::ShiftMatrix(const S21Matrix& other, int row_not,
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
