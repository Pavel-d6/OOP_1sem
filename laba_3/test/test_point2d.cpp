#include <gtest/gtest.h>
#include <sstream>
#include "Point2D.hpp"

TEST(Point2D, DefaultConstructorIsOrigin) {
    Point2D p;
    EXPECT_EQ(p.getX(), 0);
    EXPECT_EQ(p.getY(), 0);
}

TEST(Point2D, ConstructorStoresCoordinates) {
    Point2D p(3, -7);
    EXPECT_EQ(p.getX(), 3);
    EXPECT_EQ(p.getY(), -7);
}

TEST(Point2D, EqualityAndInequality) {
    EXPECT_TRUE(Point2D(1, 2) == Point2D(1, 2));
    EXPECT_FALSE(Point2D(1, 2) == Point2D(2, 1));
    EXPECT_TRUE(Point2D(1, 2) != Point2D(1, 3));
    EXPECT_FALSE(Point2D(1, 2) != Point2D(1, 2));
}

TEST(Point2D, CopyIsEqualToOriginal) {
    Point2D a(5, 6);
    Point2D b(a);
    Point2D c;
    c = a;
    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a == c);
}

TEST(Point2D, OutputFormat) {
    std::ostringstream os;
    os << Point2D(1, 2);
    EXPECT_EQ(os.str(), "(1, 2)");

    std::ostringstream os2;
    os2 << Point2D(-3, 0);
    EXPECT_EQ(os2.str(), "(-3, 0)");
}