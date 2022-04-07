#include <gtest/gtest.h>
#include "./src/main.cpp"
#include <math.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions2) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
  }

TEST(MatrizATest, MatrizACorrecta) {

  vector<vector<float>> matrizACalculada = crearMatrizA(2,4,1,10);
  float diffEntreAngulos = M_PI;
	float diffEntreRadios = (10 - 1) / 4;

}