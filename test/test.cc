#include <gtest/gtest.h>

#include "matrix_oop.h"

TEST(TestMemory, Many_rows) {
  int rows = -2;
  int cols = 3;
  try {
    Matrix M(rows, cols);
    FAIL();
  } catch (...) {
    SUCCEED();
  }
}

TEST(TestMemory, Many_cols) {
  int rows = 2;
  int cols = -3;
  try {
    Matrix M(rows, cols);
    FAIL();
  } catch (...) {
    SUCCEED();
  }
}

TEST(TestConst, Const) {
  int rows = 2;
  int cols = 3;
  const Matrix N(rows, cols);
  const Matrix M(rows, cols);
  Matrix R(rows, cols);
  ASSERT_EQ(cols, N.GetCols());
  ASSERT_EQ(rows, N.GetRows());
  ASSERT_EQ(true, N == M);

  const Matrix I = R + N;

  ASSERT_EQ(0, N(0, 0));
}

TEST(TestEqual, Error_equal) {
  int rows = 2;
  int cols = 3;
  Matrix M(rows, cols);
  Matrix N;
  int result = (M == N);
  ASSERT_FALSE(result);
  result = M.EqMatrix(N);
  ASSERT_FALSE(result);
}

TEST(TestEqual, Operator_equal) {
  int rows = 2;
  int cols = 3;
  Matrix M(rows, cols);
  Matrix N;
  N = M;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(M(i, j), N(i, j));
    }
  }
}

TEST(TestEqual, Not_equal_after_move) {
  int rows = 2;
  int cols = 3;
  Matrix M(rows, cols);
  Matrix R(rows, cols);
  Matrix N = Matrix(std::move(M));
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(R(i, j), N(i, j));
    }
  }
  ASSERT_EQ(M.GetCols(), 0);
  ASSERT_EQ(M.GetRows(), 0);
}

TEST(TestEqual, Not_equal_after_move_2) {
  int rows = 2;
  int cols = 3;
  Matrix M(rows, cols);
  Matrix R(rows, cols);
  Matrix N = std::move(M);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(R(i, j), N(i, j));
    }
  }
  ASSERT_EQ(M.GetCols(), 0);
  ASSERT_EQ(M.GetRows(), 0);
}

TEST(TestEqual, Insert_number) {
  Matrix M(1, 1);
  double num = 1.0;
  M(0, 0) = num;
  ASSERT_EQ(M(0, 0), num);
}

TEST(TestEqual, Equal_matrix) {
  Matrix M(2, 2);
  Matrix N(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  N(0, 0) = 1;
  N(0, 1) = 2;
  N(1, 0) = 3;
  N(1, 1) = 4;
  ASSERT_EQ(M, N);
  ASSERT_TRUE(M.EqMatrix(N));
}

TEST(TestEqual, Not_equal_matrix_1) {
  Matrix M(2, 2);
  Matrix N(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  N(0, 0) = 1;
  N(0, 1) = 2;
  N(1, 0) = 8;
  N(1, 1) = 4;
  ASSERT_NE(M, N);
  ASSERT_FALSE(M.EqMatrix(N));
}

TEST(TestEqual, Not_equal_matrix_2) {
  int rows = 7;
  int columns = 10;
  Matrix M(rows, columns);
  Matrix N(rows, columns);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      M(i, j) = (double)(i + j) + 0.123;
      N(i, j) = M(i, j) + 0.00000001;
    }
  }
  ASSERT_NE(M, N);
}

TEST(TestSum, Sum_operator) {
  int rows = 2;
  int cols = 2;
  Matrix M(rows, cols);
  Matrix N(rows, cols);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  N(0, 0) = 1;
  N(0, 1) = 2;
  N(1, 0) = 3;
  N(1, 1) = 4;

  Matrix RealRes(rows, cols);
  RealRes(0, 0) = 2;
  RealRes(0, 1) = 4;
  RealRes(1, 0) = 6;
  RealRes(1, 1) = 8;

  Matrix R = M + N;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(RealRes(i, j), R(i, j));
    }
  }

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_NE(M(i, j), R(i, j));
    }
  }
}

TEST(TestSum, Adding_matrix) {
  int rows = 2;
  int cols = 2;
  Matrix M(rows, cols);
  Matrix N(rows, cols);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  N(0, 0) = 1;
  N(0, 1) = 2;
  N(1, 0) = 3;
  N(1, 1) = 4;

  Matrix RealRes(rows, cols);
  RealRes(0, 0) = 2;
  RealRes(0, 1) = 4;
  RealRes(1, 0) = 6;
  RealRes(1, 1) = 8;

  M += N;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }
}

TEST(TestSum, Adding_matrix_func) {
  int rows = 2;
  int cols = 2;
  Matrix M(rows, cols);
  Matrix N(rows, cols);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  N(0, 0) = 1;
  N(0, 1) = 2;
  N(1, 0) = 3;
  N(1, 1) = 4;

  Matrix RealRes(rows, cols);
  RealRes(0, 0) = 2;
  RealRes(0, 1) = 4;
  RealRes(1, 0) = 6;
  RealRes(1, 1) = 8;

  M.SumMatrix(N);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }
}

TEST(TestSub, Sub_operator) {
  Matrix M(2, 2);
  Matrix N(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  N(0, 0) = 1;
  N(0, 1) = 2;
  N(1, 0) = 8;
  N(1, 1) = 4;

  Matrix R = M - N;
  Matrix RealRes(2, 2);
  RealRes(0, 0) = 0;
  RealRes(0, 1) = 0;
  RealRes(1, 0) = -5;
  RealRes(1, 1) = 0;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestSub, Sub_matrix_func) {
  Matrix M(2, 2);
  Matrix N(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  N(0, 0) = 1;
  N(0, 1) = 2;
  N(1, 0) = 3;
  N(1, 1) = 4;

  M -= N;

  Matrix RealRes(2, 2);
  RealRes(0, 0) = 0;
  RealRes(0, 1) = 0;
  RealRes(1, 0) = 0;
  RealRes(1, 1) = 0;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }

  M.SubMatrix(N);
  RealRes(0, 0) = -1;
  RealRes(0, 1) = -2;
  RealRes(1, 0) = -3;
  RealRes(1, 1) = -4;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }
}

TEST(TestMulNum, Mul_num_oper) {
  Matrix M(2, 2);
  Matrix N(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  double num = -1;

  Matrix R = M * num;

  Matrix RealRes(2, 2);
  RealRes(0, 0) = -1;
  RealRes(0, 1) = -2;
  RealRes(1, 0) = -3;
  RealRes(1, 1) = -4;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestMulNum, Mul_num_oper_2) {
  Matrix M(2, 2);
  Matrix N(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  double num = -1;

  Matrix R = num * M;

  Matrix RealRes(2, 2);
  RealRes(0, 0) = -1;
  RealRes(0, 1) = -2;
  RealRes(1, 0) = -3;
  RealRes(1, 1) = -4;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestMulNum, Mul_adding_to_matrix) {
  Matrix M(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  double num = -1;

  M *= num;
  Matrix RealRes(2, 2);
  RealRes(0, 0) = -1;
  RealRes(0, 1) = -2;
  RealRes(1, 0) = -3;
  RealRes(1, 1) = -4;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }

  M.MulNumber(num);
  RealRes(0, 0) = 1;
  RealRes(0, 1) = 2;
  RealRes(1, 0) = 3;
  RealRes(1, 1) = 4;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }
}

TEST(TestMatrixMul, Mul_matrix_1) {
  Matrix M(2, 2);
  Matrix N(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  N(0, 0) = 1;
  N(0, 1) = 2;
  N(1, 0) = 3;
  N(1, 1) = 4;

  M *= N;
  Matrix RealRes(2, 2);
  RealRes(0, 0) = 7;
  RealRes(0, 1) = 10;
  RealRes(1, 0) = 15;
  RealRes(1, 1) = 22;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }

  M.MulMatrix(N);
  RealRes(0, 0) = 37;
  RealRes(0, 1) = 54;
  RealRes(1, 0) = 81;
  RealRes(1, 1) = 118;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }
}

TEST(TestMatrixMul, Mul_matrix_2) {
  Matrix M(2, 3);
  M(0, 0) = 3.9;
  M(0, 1) = 0.4;
  M(0, 2) = 8345;
  M(1, 0) = 5.8354;
  M(1, 1) = 6.0;
  M(1, 2) = 24.9;

  Matrix N(3, 2);
  N(0, 0) = 138.49;
  N(0, 1) = 245.6;
  N(1, 0) = 3.0;
  N(1, 1) = 0.844;
  N(2, 0) = 0;
  N(2, 1) = 0;

  Matrix R = N * M;

  Matrix RealRes1(2, 2);
  RealRes1(0, 0) = 541.311;
  RealRes1(0, 1) = 958.1776;
  RealRes1(1, 0) = 826.144546;
  RealRes1(1, 1) = 1438.23824;

  Matrix RealRes2(3, 3);
  RealRes2(0, 0) = 1973.28524;
  RealRes2(0, 1) = 1528.996;
  RealRes2(0, 2) = 1161814.49;
  RealRes2(1, 0) = 16.6250776;
  RealRes2(1, 1) = 6.264;
  RealRes2(1, 2) = 25056.0156;
  RealRes2(2, 0) = 0;
  RealRes2(2, 1) = 0;
  RealRes2(2, 2) = 0;

  for (int i = 0; i < R.GetRows(); i++) {
    for (int j = 0; j < R.GetCols(); j++)
      ASSERT_DOUBLE_EQ(RealRes2(i, j), R(i, j));
  }
  R = M * N;
  for (int i = 0; i < R.GetRows(); i++) {
    for (int j = 0; j < R.GetCols(); j++)
      ASSERT_DOUBLE_EQ(RealRes1(i, j), R(i, j));
  }
}

TEST(TestMatrixMul, Mul_matrix_operator) {
  Matrix M(2, 2);
  Matrix N(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  N(0, 0) = 1;
  N(0, 1) = 2;
  N(1, 0) = 3;
  N(1, 1) = 4;

  Matrix R = M * N;

  Matrix RealRes(2, 2);
  RealRes(0, 0) = 7;
  RealRes(0, 1) = 10;
  RealRes(1, 0) = 15;
  RealRes(1, 1) = 22;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestTransposeMatrix, Transpose_1) {
  Matrix M(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  Matrix R = M.Transpose();
  Matrix RealRes(2, 2);
  RealRes(0, 0) = 1;
  RealRes(0, 1) = 3;
  RealRes(1, 0) = 2;
  RealRes(1, 1) = 4;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestTransposeMatrix, Transpose_2) {
  Matrix M(2, 3);
  M(0, 0) = 3.9;
  M(0, 1) = 0.4;
  M(0, 2) = 8345;
  M(1, 0) = 5.8354;
  M(1, 1) = 6.0;
  M(1, 2) = 24.9;

  Matrix R = M.Transpose();
  Matrix RealRes(3, 2);
  RealRes(0, 0) = 3.9;
  RealRes(0, 1) = 5.8354;
  RealRes(1, 0) = 0.4;
  RealRes(1, 1) = 6.0;
  RealRes(2, 0) = 8345;
  RealRes(2, 1) = 24.9;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestInverseMatrix, Inverse_1) {
  Matrix M(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  Matrix R = M.InverseMatrix();

  Matrix RealRes(2, 2);
  RealRes(0, 0) = -2;
  RealRes(0, 1) = 1;
  RealRes(1, 0) = 1.5;
  RealRes(1, 1) = -0.5;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      ASSERT_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestInverseMatrix, Inverse_2) {
  Matrix M(1, 1);
  M(0, 0) = 1;

  Matrix R = M.InverseMatrix();
  ASSERT_EQ(1, R.GetRows());
  ASSERT_EQ(1, R.GetCols());

  for (int i = 0; i < R.GetRows(); ++i) {
    for (int j = 0; j < R.GetCols(); ++j) {
      ASSERT_DOUBLE_EQ(1, R(i, j));
    }
  }
}

TEST(TestInverseMatrix, Inverse_3) {
  Matrix M(3, 3);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(0, 2) = 2;
  M(1, 0) = 3;
  M(1, 1) = 5;
  M(1, 2) = 5;
  M(2, 0) = 3;
  M(2, 1) = 5;
  M(2, 2) = 6;

  Matrix R = M.InverseMatrix();

  Matrix RealRes(3, 3);
  RealRes(0, 0) = -5;
  RealRes(0, 1) = 2;
  RealRes(0, 2) = 0;
  RealRes(1, 0) = 3;
  RealRes(1, 1) = 0;
  RealRes(1, 2) = -1;
  RealRes(2, 0) = 0;
  RealRes(2, 1) = -1;
  RealRes(2, 2) = 1;

  for (int i = 0; i < R.GetRows(); ++i) {
    for (int j = 0; j < R.GetCols(); ++j) {
      ASSERT_DOUBLE_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestInverseMatrix, Inverse_loop) {
  Matrix M(3, 3);
  M(0, 0) = rand();
  M(0, 1) = rand();
  M(0, 2) = rand();
  M(1, 0) = rand();
  M(1, 1) = rand();
  M(1, 2) = rand();
  M(2, 0) = rand();
  M(2, 1) = rand();
  M(2, 2) = rand();

  Matrix Res1 = M.InverseMatrix();
  M.MulMatrix(Res1);

  ASSERT_NEAR(M(0, 0), 1, kEpsilon);
  ASSERT_NEAR(M(0, 1), 0, kEpsilon);
  ASSERT_NEAR(M(0, 2), 0, kEpsilon);
  ASSERT_NEAR(M(1, 0), 0, kEpsilon);
  ASSERT_NEAR(M(1, 1), 1, kEpsilon);
  ASSERT_NEAR(M(1, 2), 0, kEpsilon);
  ASSERT_NEAR(M(2, 0), 0, kEpsilon);
  ASSERT_NEAR(M(2, 1), 0, kEpsilon);
  ASSERT_NEAR(M(2, 2), 1, kEpsilon);
}

TEST(TestDeterminant, Det_1) {
  Matrix M(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  double R = M.Determinant();
  ASSERT_DOUBLE_EQ(R, -2.000000);
}

TEST(TestDeterminant, Det_2) {
  Matrix M(2, 2);
  M(0, 0) = 109.56;
  M(0, 1) = 25;
  M(1, 0) = 334.684;
  M(1, 1) = 0.0;

  double R = M.Determinant();
  ASSERT_DOUBLE_EQ(R, -8367.1);
}

TEST(TestDeterminant, Det_3) {
  Matrix M(3, 3);
  M(0, 0) = 109.56;
  M(0, 1) = 25;
  M(0, 2) = 334.684;
  M(1, 0) = 0.0;
  M(1, 1) = 0;
  M(1, 2) = 34.86;
  M(2, 0) = 7;
  M(2, 1) = 8;
  M(2, 2) = 9.0;

  double R = M.Determinant();
  ASSERT_DOUBLE_EQ(R, -24453.5928);
}

TEST(TestDeterminant, Det_4) {
  Matrix M(1, 1);
  M(0, 0) = 109.56;

  double R = M.Determinant();
  ASSERT_DOUBLE_EQ(R, 109.56);
}

TEST(TestCalcComp, Matrix_3) {
  Matrix M(3, 3);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(0, 2) = 3;
  M(1, 0) = 0;
  M(1, 1) = 4;
  M(1, 2) = 2;
  M(2, 0) = 5;
  M(2, 1) = 2;
  M(2, 2) = 1;

  Matrix R = M.CalcComplements();

  Matrix RealRes(3, 3);
  RealRes(0, 0) = 0;
  RealRes(0, 1) = 10;
  RealRes(0, 2) = -20;
  RealRes(1, 0) = 4;
  RealRes(1, 1) = -14;
  RealRes(1, 2) = 8;
  RealRes(2, 0) = -8;
  RealRes(2, 1) = -2;
  RealRes(2, 2) = 4;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      ASSERT_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestCalcComp, Matrix_1) {
  Matrix M(1, 1);
  M(0, 0) = 1;

  Matrix R = M.CalcComplements();

  for (int i = 0; i < M.GetRows(); ++i) {
    for (int j = 0; j < M.GetCols(); ++j) {
      ASSERT_DOUBLE_EQ(1, R(i, j));
    }
  }
}

TEST(TestCalcComp, Matrix_4) {
  Matrix M(4, 4);
  M(0, 0) = 9;
  M(0, 1) = 7;
  M(0, 2) = 0;
  M(0, 3) = 0;
  M(1, 0) = 7.3;
  M(1, 1) = 0.0;
  M(1, 2) = 0;
  M(1, 3) = 0;
  M(2, 0) = 0;
  M(2, 1) = 0;
  M(2, 2) = 5;
  M(2, 3) = 0;
  M(3, 0) = 0;
  M(3, 1) = 4;
  M(3, 2) = 0;
  M(3, 3) = 10;

  Matrix R = M.CalcComplements();

  Matrix RealRes(4, 4);
  RealRes(0, 0) = 0;
  RealRes(0, 1) = -365;
  RealRes(0, 2) = 0;
  RealRes(0, 3) = 146;
  RealRes(1, 0) = -350;
  RealRes(1, 1) = 450;
  RealRes(1, 2) = 0;
  RealRes(1, 3) = -180;
  RealRes(2, 0) = 0;
  RealRes(2, 1) = 0;
  RealRes(2, 2) = -511;
  RealRes(2, 3) = 0;
  RealRes(3, 0) = 0;
  RealRes(3, 1) = 0;
  RealRes(3, 2) = 0;
  RealRes(3, 3) = -255.5;

  for (int i = 0; i < M.GetRows(); ++i) {
    for (int j = 0; j < M.GetCols(); ++j) {
      ASSERT_DOUBLE_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestOperations, Different_operations) {
  Matrix M(1, 1);
  M(0, 0) = 1;
  Matrix N(1, 1);
  N(0, 0) = 2;
  Matrix T(1, 4);
  T(0, 0) = 1;
  T(0, 0) = 2;
  T(0, 0) = 3;
  T(0, 0) = 4;

  Matrix RealRes(1, 4);
  RealRes(0, 0) = 2;
  RealRes(0, 0) = 4;
  RealRes(0, 0) = 6;
  RealRes(0, 0) = 8;

  Matrix R = (M + N) * T - T;

  for (int i = 0; i < R.GetRows(); ++i) {
    for (int j = 0; j < R.GetCols(); ++j) {
      ASSERT_DOUBLE_EQ(RealRes(i, j), R(i, j));
    }
  }
}

TEST(TestExcept, Different_matrix) {
  int rows = 1;
  int cols = 1;
  Matrix M;
  Matrix N(rows, cols);
  N(0, 0) = 1;

  try {
    Matrix R = M + N;
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("Different matrix dimensions.", ex.what());
  }
}

TEST(TestExcept, Different_matrix_2) {
  int rows = 1;
  int cols = 1;
  Matrix M(rows + 1, cols);
  Matrix N(rows, cols);
  N(0, 0) = 1;

  try {
    Matrix R = M - N;
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("Different matrix dimensions.", ex.what());
  }
}

TEST(TestExcept, Different_matrix_3) {
  int rows = 1;
  int cols = 1;
  Matrix M(rows + 1, cols);
  Matrix N(rows, cols);
  N(0, 0) = 1;

  try {
    Matrix R = M + N;
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("Different matrix dimensions.", ex.what());
  }
}

TEST(TestExcept, Initialised_matrix) {
  Matrix M;
  try {
    Matrix N = M.CalcComplements();
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("The matrix is not initialised.", ex.what());
  }
}

TEST(TestExcept, Index_out_of_range) {
  Matrix M;
  try {
    M(1, 1);
    FAIL();
  } catch (std::out_of_range& ex) {
    EXPECT_STREQ("Index out of range.", ex.what());
  }
}

TEST(TestExcept, Index_out_of_range_const) {
  const Matrix M;
  try {
    M(1, 1);
    FAIL();
  } catch (std::out_of_range& ex) {
    EXPECT_STREQ("Index out of range.", ex.what());
  }
}

TEST(TestExcept, Mul_matrix) {
  Matrix M(1, 2);
  Matrix N(1, 2);
  try {
    M *= N;
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ(
        "The number of columns of the first matrix is not equal to the number "
        "of rows of the second matrix.",
        ex.what());
  }
}

TEST(TestExcept, Sub_matrix) {
  Matrix M(1, 2);
  Matrix N(1, 1);
  try {
    M -= N;
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("Different matrix dimensions.", ex.what());
  }
}

TEST(TestExcept, Determinant_less_than_zero) {
  Matrix M(1, 1);
  M(0, 0) = 0;

  try {
    M = M.InverseMatrix();
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("The matrix determinant is 0.", ex.what());
  }
}

TEST(TestExcept, Not_square_matrix_determinant) {
  Matrix M(1, 2);
  M(0, 0) = 2;
  M(0, 1) = 3;

  try {
    M.Determinant();
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("The matrix is not square.", ex.what());
  }
}

TEST(TestExcept, Not_square_matrix_calc_complements) {
  Matrix M(1, 2);
  M(0, 0) = 2;
  M(0, 1) = 3;

  try {
    Matrix N = M.CalcComplements();
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("The matrix is not square.", ex.what());
  }
}

TEST(TestExcept, Set_cols_less_than_zero) {
  Matrix M(1, 2);

  try {
    M.SetCols(-3);
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("Number of columns less than 0.", ex.what());
  }
}

TEST(TestExcept, Set_rows_less_than_zero) {
  Matrix M(1, 2);

  try {
    M.SetRows(-3);
    FAIL();
  } catch (std::invalid_argument& ex) {
    EXPECT_STREQ("Number of rows less than 0.", ex.what());
  }
}

TEST(TestMutator, Set_rows_less) {
  Matrix M(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  M.SetRows(1);

  int rows = 1;
  int cols = 2;
  Matrix RealRes(rows, cols);
  RealRes(0, 0) = 1;
  RealRes(0, 1) = 2;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }
}

TEST(TestMutator, Set_rows_more) {
  Matrix M(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  M.SetRows(3);

  int rows = 3;
  int cols = 2;
  Matrix RealRes(rows, cols);
  RealRes(0, 0) = 1;
  RealRes(0, 1) = 2;
  RealRes(1, 0) = 3;
  RealRes(1, 1) = 4;
  RealRes(2, 0) = 0;
  RealRes(2, 1) = 0;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }
}

TEST(TestMutator, Set_cols_more) {
  Matrix M(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  int rows = 2;
  int cols = 3;
  M.SetCols(cols);

  Matrix RealRes(rows, cols);
  RealRes(0, 0) = 1;
  RealRes(0, 1) = 2;
  RealRes(0, 2) = 0;
  RealRes(1, 0) = 3;
  RealRes(1, 1) = 4;
  RealRes(1, 2) = 0;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }
}

TEST(TestMutator, Set_cols_less) {
  Matrix M(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 2;
  M(1, 0) = 3;
  M(1, 1) = 4;

  int rows = 2;
  int cols = 1;
  M.SetCols(cols);

  Matrix RealRes(rows, cols);
  RealRes(0, 0) = 1;
  RealRes(1, 0) = 3;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      ASSERT_EQ(RealRes(i, j), M(i, j));
    }
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}