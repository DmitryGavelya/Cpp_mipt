#include <gtest/gtest.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;
using ull = unsigned long long int;

matrix<ull> multiply(const matrix<ull>& A, const matrix<ull>& B) {
  matrix<ull> C(2, 2);
  C(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
  C(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
  C(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
  C(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
  return C;
}

matrix<ull> power(matrix<ull> M, ull p) {
  matrix<ull> res(2, 2);
  res(0, 0) = 1; res(0, 1) = 0;
  res(1, 0) = 0; res(1, 1) = 1;

  while (p > 0) {
    if (p % 2 == 1) {
      res = multiply(res, M);
    }
    M = multiply(M, M);
    p /= 2;
  }
  return res;
}

ull fibonacci(ull n) {
  if (n == 0) return 0;
  if (n == 1) return 1;

  matrix<ull> T(2, 2);
  T(0, 0) = 1; T(0, 1) = 1;
  T(1, 0) = 1; T(1, 1) = 0;

  T = power(T, n - 1);

  return T(0, 0);
}

TEST(FibonacciTest, BaseCases) {
  EXPECT_EQ(fibonacci(0), 0);
  EXPECT_EQ(fibonacci(1), 1);
  EXPECT_EQ(fibonacci(2), 1);
  EXPECT_EQ(fibonacci(3), 2);
  EXPECT_EQ(fibonacci(4), 3);
  EXPECT_EQ(fibonacci(5), 5);
}

TEST(FibonacciTest, LargerValues) {
  EXPECT_EQ(fibonacci(10), 55);
  EXPECT_EQ(fibonacci(20), 6765);
  EXPECT_EQ(fibonacci(50), 12586269025ULL);
  EXPECT_EQ(fibonacci(93), 12200160415121876738ULL);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
