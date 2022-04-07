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
  vector<vector<float>> matrizAMano = {{coeficienteB(diffEntreAngulos, diffEntreRadios, 1, 1), coeficienteC(diffEntreRadios), 0.0, 0.0, coeficienteD(diffEntreAngulos, diffEntreRadios, 1, 1), 0.0, 0.0, 0.0},
                                       {coeficienteA(diffEntreRadios, 1, 1), coeficienteB(diffEntreAngulos, diffEntreRadios, 2, 1), coeficienteC(diffEntreRadios), 0.0, 0.0, coeficienteD(diffEntreAngulos, diffEntreRadios, 2, 1), 0.0, 0.0},
                                       {0.0, coeficienteA(diffEntreRadios, 2, 1), coeficienteB(diffEntreAngulos, diffEntreRadios, 3, 1), coeficienteC(diffEntreRadios), 0.0, 0.0, coeficienteD(diffEntreAngulos, diffEntreRadios, 3, 1), 0.0},
                                       {0.0, 0.0, coeficienteA(diffEntreRadios, 3, 1), coeficienteB(diffEntreAngulos, diffEntreRadios, 4, 1), 0.0, 0.0, 0.0, coeficienteD(diffEntreAngulos, diffEntreRadios, 4, 1)},
                                       {coeficienteD(diffEntreAngulos, diffEntreRadios, 1, 1), 0.0, 0.0, 0.0, coeficienteB(diffEntreAngulos, diffEntreRadios, 1, 1), coeficienteC(diffEntreRadios), 0.0, 0.0},
                                       {0.0, coeficienteD(diffEntreAngulos, diffEntreRadios, 2, 1), 0.0, 0.0, coeficienteA(diffEntreRadios, 1, 1), coeficienteB(diffEntreAngulos, diffEntreRadios, 2, 1), coeficienteC(diffEntreRadios), 0.0},
                                       {0.0, 0.0, coeficienteD(diffEntreAngulos, diffEntreRadios, 3, 1), 0.0, 0.0, coeficienteA(diffEntreRadios, 2, 1), coeficienteB(diffEntreAngulos, diffEntreRadios, 3, 1), coeficienteC(diffEntreRadios)},
                                       {0.0, 0.0, 0.0, coeficienteD(diffEntreAngulos, diffEntreRadios, 4, 1), 0.0, 0.0, coeficienteA(diffEntreRadios, 3, 1), coeficienteB(diffEntreAngulos, diffEntreRadios, 4, 1)}};
  
  for(int i = 0; i<8; i++){
    for(int j = 0; j<8; j++){
      EXPECT_EQ(matrizAMano[i][j], matrizACalculada[i][j]);
    }
  }

}