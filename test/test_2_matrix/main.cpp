/*
to run all the test use the following command
pio test -e native
*/

#include <unity.h>
#include <matrix.hpp>
using namespace operators;
#ifdef NATIVE
template <typename T>
void print(const Vector<T> &v) {
    std::cout << to_string(v) << std::endl;
}
#else
template <typename T>
void print(const Vector<T> &v) {
    Serial.println(to_string(v).c_str());
}
#endif

void test_ctor_dtor(void) {
    Matrix<double> m;
    TEST_ASSERT_EQUAL(0, m.rows());
    TEST_ASSERT_EQUAL(0, m.cols());
    TEST_ASSERT_EQUAL(0, m.size());
    TEST_ASSERT_EQUAL(0, m.capacity());
    TEST_ASSERT_EQUAL(0, m.T.rows());
    TEST_ASSERT_EQUAL(0, m.T.cols());
    TEST_ASSERT_EQUAL(0, m.T.size());
    TEST_ASSERT_EQUAL(0, m.T.capacity());

    Matrix<int> m1(2, 3);
    m1.fill(1);
    m1(0,1) = 2;
    TEST_ASSERT_EQUAL(2, m1.rows());
    TEST_ASSERT_EQUAL(3, m1.cols());
    TEST_ASSERT_EQUAL(6, m1.size());
    TEST_ASSERT_EQUAL(6, m1.capacity());
    TEST_ASSERT_EQUAL(3, m1.T.rows());
    TEST_ASSERT_EQUAL(2, m1.T.cols());
    TEST_ASSERT_EQUAL(6, m1.T.size());
    TEST_ASSERT_TRUE(m1.T.shared());
}

void test_dataType(void) {
    Matrix<int> m1(2, 3);
    m1.fill(1);
    TEST_ASSERT_EQUAL(1, m1(0,0));
    TEST_ASSERT_EQUAL(1, m1(0,1));
    TEST_ASSERT_EQUAL(1, m1(0,2));
    TEST_ASSERT_EQUAL(1, m1(1,0));
    TEST_ASSERT_EQUAL(1, m1(1,1));
    TEST_ASSERT_EQUAL(1, m1(1,2));
    m1(0,1) = 2;
    m1 += 1;
    TEST_ASSERT_EQUAL(2, m1(0,0));
    TEST_ASSERT_EQUAL(3, m1(0,1));
    TEST_ASSERT_EQUAL(2, m1(0,2));
    TEST_ASSERT_EQUAL(2, m1(1,0));
    TEST_ASSERT_EQUAL(2, m1(1,1));
    TEST_ASSERT_EQUAL(2, m1(1,2));
    m1 -= 1;
    TEST_ASSERT_EQUAL(1, m1(0,0));
    TEST_ASSERT_EQUAL(2, m1(0,1));
    TEST_ASSERT_EQUAL(1, m1(0,2));
    TEST_ASSERT_EQUAL(1, m1(1,0));
    TEST_ASSERT_EQUAL(1, m1(1,1));
    TEST_ASSERT_EQUAL(1, m1(1,2));
    m1 *= 2;
    TEST_ASSERT_EQUAL(2, m1(0,0));
    TEST_ASSERT_EQUAL(4, m1(0,1));
    TEST_ASSERT_EQUAL(2, m1(0,2));
    TEST_ASSERT_EQUAL(2, m1(1,0));
    TEST_ASSERT_EQUAL(2, m1(1,1));
    TEST_ASSERT_EQUAL(2, m1(1,2));
    m1 /= 2;
    TEST_ASSERT_EQUAL(1, m1(0,0));
    TEST_ASSERT_EQUAL(2, m1(0,1));
    TEST_ASSERT_EQUAL(1, m1(0,2));
    TEST_ASSERT_EQUAL(1, m1(1,0));
    TEST_ASSERT_EQUAL(1, m1(1,1));
    TEST_ASSERT_EQUAL(1, m1(1,2));
}

void test_rowMajor(void) {
    Matrix<int> m1(2, 3);
    rowMajorMatrix<int> m2(2, 3);
    m2.fill(1);
    m1 = m2;
    TEST_ASSERT_EQUAL(2, m1.rows());
    TEST_ASSERT_EQUAL(3, m1.cols());
    TEST_ASSERT_EQUAL(6, m1.size());
    TEST_ASSERT_EQUAL(6, m1.capacity());
    TEST_ASSERT_EQUAL(3, m1.T.rows());
    TEST_ASSERT_EQUAL(2, m1.T.cols());
    TEST_ASSERT_EQUAL(6, m1.T.size());
    TEST_ASSERT_TRUE(m1.T.shared());
    TEST_ASSERT_EQUAL(1, m1(0,0));
    TEST_ASSERT_EQUAL(1, m1(0,1));
    TEST_ASSERT_EQUAL(1, m1(0,2));
    TEST_ASSERT_EQUAL(1, m1(1,0));
    TEST_ASSERT_EQUAL(1, m1(1,1));
    TEST_ASSERT_EQUAL(1, m1(1,2));
}

void test_tmp_rowMajor(void) {
    Matrix<int> m1(2, 3);
    rowMajorMatrix<int> m2(2, 3);
    m2.fill(1);
    m1 = 2*m2;
    TEST_ASSERT_EQUAL(2, m1.rows());
    TEST_ASSERT_EQUAL(3, m1.cols());
    TEST_ASSERT_EQUAL(6, m1.size());
    TEST_ASSERT_EQUAL(6, m1.capacity());
    TEST_ASSERT_EQUAL(3, m1.T.rows());
    TEST_ASSERT_EQUAL(2, m1.T.cols());
    TEST_ASSERT_EQUAL(6, m1.T.size());
    TEST_ASSERT_TRUE(m1.T.shared());
    TEST_ASSERT_EQUAL(2, m1(0,0));
    TEST_ASSERT_EQUAL(2, m1(0,1));
    TEST_ASSERT_EQUAL(2, m1(0,2));
    TEST_ASSERT_EQUAL(2, m1(1,0));
    TEST_ASSERT_EQUAL(2, m1(1,1));
    TEST_ASSERT_EQUAL(2, m1(1,2));
    TEST_ASSERT_EQUAL(1, internal::tmp<rowMajorMatrix<int>>::bufferSize());
    TEST_ASSERT_EQUAL(0, internal::tmp<rowMajorMatrix<int>>::currentlyUsedCount());
}

void test_expression(void) {
    Matrix<int> m1;
    rowMajorMatrix<int> m2(2, 3);
    m2.fill(1);
    m2(0,1) = 2;
    colMajorMatrix<int> m3(3, 2);
    m3.fill(1);
    m3(1,0) = 2;

    m1 = m2*m3;
    TEST_ASSERT_EQUAL(2, m1.rows());
    TEST_ASSERT_EQUAL(2, m1.cols());
    TEST_ASSERT_EQUAL(4, m1.size());
    TEST_ASSERT_EQUAL(6, m1.capacity());
    TEST_ASSERT_EQUAL(6, m1(0,0));
    TEST_ASSERT_EQUAL(4, m1(0,1));
    TEST_ASSERT_EQUAL(4, m1(1,0));
    TEST_ASSERT_EQUAL(3, m1(1,1));
}

void test_col_col_mult(void) {
    Matrix<int> m1;
    colMajorMatrix<int> m2(2, 3);
    m2.fill(1);
    m2(0,1) = 2;
    colMajorMatrix<int> m3(3, 2);
    m3.fill(1);
    m3(1,0) = 2;

    m1 = m2*m3;
    TEST_ASSERT_EQUAL(2, m1.rows());
    TEST_ASSERT_EQUAL(2, m1.cols());
    TEST_ASSERT_EQUAL(4, m1.size());
    TEST_ASSERT_EQUAL(4, m1.capacity());
    TEST_ASSERT_EQUAL(6, m1(0,0));
    TEST_ASSERT_EQUAL(4, m1(0,1));
    TEST_ASSERT_EQUAL(4, m1(1,0));
    TEST_ASSERT_EQUAL(3, m1(1,1));
}


void setUp() {
    // Initialisation avant chaque test (laisser vide si inutile)
}

void tearDown() {
    // Nettoyage après chaque test (laisser vide si inutile)
}


void setup() {
    UNITY_BEGIN();
    // Ajoute d'autres tests ici.
    RUN_TEST(test_ctor_dtor);
    RUN_TEST(test_dataType);
    RUN_TEST(test_rowMajor);
    RUN_TEST(test_tmp_rowMajor);
    RUN_TEST(test_expression);
    RUN_TEST(test_col_col_mult);
    UNITY_END();
}

void loop() {
    // Vide car Unity fonctionne avec setup() pour exécuter tous les tests une fois.
}

#ifdef NATIVE
int main(int argc, char **argv) {
    setup();
    return UNITY_END();
}
#endif