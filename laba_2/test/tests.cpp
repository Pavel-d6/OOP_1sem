#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include "Polynomial.hpp"

static std::string str(const Polynomial& p) {
    std::ostringstream os;
    p.print(os);
    return os.str();
}

TEST(Polynomial, DefaultIsZero) {
    Polynomial p;
    EXPECT_EQ(p.degree(), -1);
    EXPECT_DOUBLE_EQ(p.evaluate(5.0), 0.0);
    EXPECT_EQ(str(p), "0");
}

TEST(Polynomial, ConstructFromArray) {
    double c[] = {1.0, 2.0, 3.0};
    Polynomial p(c, 2);
    EXPECT_EQ(p.degree(), 2);
    EXPECT_DOUBLE_EQ(p.coeff(1), 2.0);
    EXPECT_DOUBLE_EQ(p.coeff(7), 0.0);
    EXPECT_DOUBLE_EQ(p.coeff(-1), 0.0);
}

TEST(Polynomial, ConstructorTrimsLeadingZeros) {
    double c[] = {1.0, 2.0, 0.0, 0.0};
    Polynomial p(c, 3);
    EXPECT_EQ(p.degree(), 1);
    double z[] = {0.0, 0.0};
    Polynomial q(z, 1);
    EXPECT_EQ(q.degree(), -1);
}

TEST(Polynomial, ZeroInMiddleIsKept) {
    double c[] = {1.0, 0.0, 3.0};
    Polynomial p(c, 2);
    EXPECT_EQ(p.degree(), 2);
    EXPECT_DOUBLE_EQ(p.coeff(1), 0.0);
}

TEST(Polynomial, InvalidArgumentsThrow) {
    EXPECT_THROW(Polynomial(nullptr, 2), std::invalid_argument);
    double c[] = {1.0};
    EXPECT_THROW(Polynomial(c, -5), std::invalid_argument);
    EXPECT_NO_THROW(Polynomial(nullptr, -1));
}

TEST(Polynomial, CopyIsDeep) {
    double c[] = {1.0, 2.0};
    Polynomial a(c, 1);
    Polynomial b(a);
    double d[] = {5.0};
    b = Polynomial(d, 0);
    EXPECT_EQ(a.degree(), 1);
    EXPECT_DOUBLE_EQ(a.coeff(1), 2.0);
}

TEST(Polynomial, CopyOfZero) {
    Polynomial a;
    Polynomial b(a);
    EXPECT_EQ(b.degree(), -1);
}

TEST(Polynomial, MoveLeavesSourceZero) {
    double c[] = {1.0, 2.0};
    Polynomial a(c, 1);
    Polynomial b(std::move(a));
    EXPECT_EQ(b.degree(), 1);
    EXPECT_EQ(a.degree(), -1);
    EXPECT_DOUBLE_EQ(a.evaluate(3.0), 0.0);
}

TEST(Polynomial, SelfAssignment) {
    double c[] = {1.0, 2.0, 3.0};
    Polynomial a(c, 2);
    Polynomial& ref = a;
    a = ref;
    EXPECT_EQ(a.degree(), 2);
    EXPECT_DOUBLE_EQ(a.coeff(2), 3.0);
}

TEST(Polynomial, CopyAndMoveAssignment) {
    double c[] = {1.0, 2.0};
    Polynomial a(c, 1), b, d;
    b = a;
    EXPECT_EQ(b.degree(), 1);
    d = std::move(b);
    EXPECT_EQ(d.degree(), 1);
    EXPECT_EQ(b.degree(), -1);
}

TEST(Polynomial, AddAndEvaluate) {
    double c1[] = {1.0, 2.0}, c2[] = {0.0, 1.0, 1.0};
    Polynomial s(c1, 1), b(c2, 2);
    s.add(b);
    EXPECT_EQ(s.degree(), 2);
    EXPECT_DOUBLE_EQ(s.evaluate(2.0), 11.0);
}

TEST(Polynomial, AddCancelsToZero) {
    double c[] = {1.0, 2.0}, m[] = {-1.0, -2.0};
    Polynomial a(c, 1), b(m, 1);
    a.add(b);
    EXPECT_EQ(a.degree(), -1);
}

TEST(Polynomial, AddZeroAndAddToZero) {
    double c[] = {1.0, 2.0};
    Polynomial a(c, 1), z;
    a.add(z);
    EXPECT_EQ(a.degree(), 1);
    z.add(a);
    EXPECT_EQ(z.degree(), 1);
    EXPECT_DOUBLE_EQ(z.coeff(1), 2.0);
}

TEST(Polynomial, MulMatchesExample) {
    double c1[] = {1.0, 2.0}, c2[] = {0.0, 1.0, 1.0};
    Polynomial m(c1, 1), b(c2, 2);
    m.mul(b);
    EXPECT_EQ(m.degree(), 3);
    EXPECT_DOUBLE_EQ(m.coeff(0), 0.0);
    EXPECT_DOUBLE_EQ(m.coeff(1), 1.0);
    EXPECT_DOUBLE_EQ(m.coeff(2), 3.0);
    EXPECT_DOUBLE_EQ(m.coeff(3), 2.0);
}

TEST(Polynomial, MulByZeroGivesZero) {
    double c[] = {1.0, 2.0};
    Polynomial m(c, 1);
    m.mul(Polynomial());
    EXPECT_EQ(m.degree(), -1);
    Polynomial z;
    z.mul(m);
    EXPECT_EQ(z.degree(), -1);
}

TEST(Polynomial, MulBySelf) {
    double c[] = {1.0, 1.0};
    Polynomial p(c, 1);
    p.mul(p);
    EXPECT_EQ(p.degree(), 2);
    EXPECT_DOUBLE_EQ(p.coeff(1), 2.0);
}

TEST(Polynomial, PrintFormat) {
    double c[] = {1.0, -3.0, 1.0};
    EXPECT_EQ(str(Polynomial(c, 2)), "1 - 3x + x^2");
}

TEST(Polynomial, Derivative) {
    double c[] = {1.0, 2.0, 3.0};
    Polynomial d = Polynomial(c, 2).derivative();
    EXPECT_EQ(d.degree(), 1);
    EXPECT_DOUBLE_EQ(d.coeff(0), 2.0);
    EXPECT_DOUBLE_EQ(d.coeff(1), 6.0);
}

TEST(Polynomial, DerivativeOfConstantAndZero) {
    double c[] = {7.0};
    EXPECT_EQ(Polynomial(c, 0).derivative().degree(), -1);
    EXPECT_EQ(Polynomial().derivative().degree(), -1);
}

TEST(Polynomial, Integral) {
    double c[] = {0.0, 2.0};
    Polynomial i = Polynomial(c, 1).integral();
    EXPECT_EQ(i.degree(), 2);
    EXPECT_DOUBLE_EQ(i.coeff(0), 0.0);
    EXPECT_DOUBLE_EQ(i.coeff(1), 0.0);
    EXPECT_DOUBLE_EQ(i.coeff(2), 1.0);
}

TEST(Polynomial, IntegralOfZero) {
    EXPECT_EQ(Polynomial().integral().degree(), -1);
}

TEST(Polynomial, DerivativeOfIntegralIsOriginal) {
    double c[] = {3.0, -1.0, 4.0, 2.0};
    Polynomial p(c, 3);
    Polynomial back = p.integral().derivative();
    ASSERT_EQ(back.degree(), 3);
    for (int k = 0; k <= 3; ++k) {
        EXPECT_NEAR(back.coeff(k), p.coeff(k), 1e-12);
    }
}
