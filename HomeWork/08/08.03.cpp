#include <cassert>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <gtest/gtest.h>

int ilog2_int(int x) {
  if (x <= 0) return -1;
  unsigned int u = static_cast<unsigned int>(x);

  int res = -1;
  while (u) {
    u >>= 1;
    ++res;
  }
  return res;
}

int ilog2_float(float f) {
  union {
    float f;
    std::uint32_t u;
  } v;

  v.f = f;

  std::uint32_t sign = v.u >> 31;
  std::uint32_t exp  = (v.u >> 23) & 0xFFu;
  std::uint32_t mant =  v.u & 0x7FFFFFu;

  if (sign || exp == 0xFFu || (exp == 0 && mant == 0))
    return -1;

  if (exp == 0) {
    int shift = 0;
    while ((mant & (1u << 22)) == 0) {
      mant <<= 1;
      ++shift;
      if (shift > 23) return -1;
    }
    int e = 1 - 127 - shift;
    return e;
  } else {
    int e = static_cast<int>(exp) - 127;
    return e;
  }
}

TEST(Ilog2IntTest, BasicCases) {
  EXPECT_EQ(ilog2_int(1), 0);
  EXPECT_EQ(ilog2_int(2), 1);
  EXPECT_EQ(ilog2_int(8), 3);
  EXPECT_EQ(ilog2_int(1024), 10);
}

TEST(Ilog2FloatTest, BasicCases) {
  EXPECT_EQ(ilog2_float(1.0f), 0);
  EXPECT_EQ(ilog2_float(2.0f), 1);
  EXPECT_EQ(ilog2_float(0.5f), -1);
  EXPECT_EQ(ilog2_float(0.25f), -2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
