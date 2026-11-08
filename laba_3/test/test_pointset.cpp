#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <utility>
#include "PointSet.hpp"

namespace {
std::string str(const PointSet& s) {
    std::ostringstream os;
    os << s;
    return os.str();
}
} 


TEST(PointSet, EmptySet) {
    PointSet s;
    EXPECT_EQ(s.size(), 0u);
    EXPECT_TRUE(s.empty());
    EXPECT_FALSE(s.contains(Point2D(0, 0)));
}


TEST(PointSet, AddNewAndDuplicate) {
    PointSet s;
    EXPECT_TRUE(s.add(Point2D(1, 2)));
    EXPECT_FALSE(s.add(Point2D(1, 2)));
    EXPECT_TRUE(s.add(Point2D(2, 1)));
    EXPECT_EQ(s.size(), 2u);
    EXPECT_FALSE(s.empty());
}


TEST(PointSet, RemoveExistingAndMissing) {
    PointSet s;
    s.add(Point2D(1, 1));
    s.add(Point2D(2, 2));
    s.add(Point2D(3, 3));
    EXPECT_TRUE(s.remove(Point2D(2, 2)));
    EXPECT_FALSE(s.remove(Point2D(2, 2)));
    EXPECT_EQ(s.size(), 2u);
    EXPECT_FALSE(s.contains(Point2D(2, 2)));
    EXPECT_EQ(str(s), "{ (1, 1), (3, 3) }");  
}

TEST(PointSet, RemoveLastAndFromEmpty) {
    PointSet s;
    EXPECT_FALSE(s.remove(Point2D(1, 1)));
    s.add(Point2D(1, 1));
    EXPECT_TRUE(s.remove(Point2D(1, 1)));
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(str(s), "{ }");
}


TEST(PointSet, ContainsAndCallOperatorAgree) {
    PointSet s;
    s.add(Point2D(1, 2));
    EXPECT_TRUE(s.contains(Point2D(1, 2)));
    EXPECT_TRUE(s(Point2D(1, 2)));
    EXPECT_FALSE(s.contains(Point2D(2, 1)));
    EXPECT_FALSE(s(Point2D(2, 1)));
}

TEST(PointSet, PlusMinusAssignChain) {
    PointSet s;
    (s += Point2D(1, 1)) += Point2D(2, 2);
    s += Point2D(3, 3);
    EXPECT_EQ(s.size(), 3u);
    (s -= Point2D(1, 1)) -= Point2D(2, 2);
    EXPECT_EQ(s.size(), 1u);
    EXPECT_TRUE(s(Point2D(3, 3)));
}

TEST(PointSet, CopyConstructorIsIndependent) {
    PointSet a;
    a.add(Point2D(1, 1));
    a.add(Point2D(2, 2));
    PointSet b(a);
    b.add(Point2D(3, 3));
    b.remove(Point2D(1, 1));

    EXPECT_EQ(a.size(), 2u);
    EXPECT_TRUE(a.contains(Point2D(1, 1)));
    EXPECT_FALSE(a.contains(Point2D(3, 3)));
    EXPECT_EQ(b.size(), 2u);
}

TEST(PointSet, CopyAssignmentIsIndependent) {
    PointSet a;
    a.add(Point2D(1, 1));
    a.add(Point2D(2, 2));
    PointSet b;
    b.add(Point2D(9, 9));
    b = a;
    b.add(Point2D(3, 3));

    EXPECT_EQ(a.size(), 2u);
    EXPECT_FALSE(a.contains(Point2D(3, 3)));
    EXPECT_EQ(b.size(), 3u);
    EXPECT_FALSE(b.contains(Point2D(9, 9)));
}

TEST(PointSet, SelfAssignment) {
    PointSet a;
    a.add(Point2D(1, 1));
    PointSet& ref = a;
    a = ref;
    EXPECT_EQ(a.size(), 1u);
    EXPECT_TRUE(a.contains(Point2D(1, 1)));
}

TEST(PointSet, CopyOfEmpty) {
    PointSet a;
    PointSet b(a);
    EXPECT_TRUE(b.empty());
    b.add(Point2D(1, 1));
    EXPECT_EQ(b.size(), 1u);
    EXPECT_TRUE(a.empty());
}


TEST(PointSet, MoveConstructorLeavesSourceEmpty) {
    PointSet a;
    a.add(Point2D(1, 1));
    a.add(Point2D(2, 2));
    PointSet b(std::move(a));

    EXPECT_EQ(b.size(), 2u);
    EXPECT_TRUE(b.contains(Point2D(2, 2)));
    EXPECT_EQ(a.size(), 0u);
    EXPECT_TRUE(a.empty());
    a.add(Point2D(5, 5));  
    EXPECT_EQ(a.size(), 1u);
}

TEST(PointSet, MoveAssignmentLeavesSourceEmpty) {
    PointSet a;
    a.add(Point2D(1, 1));
    PointSet b;
    b.add(Point2D(9, 9));
    b = std::move(a);

    EXPECT_EQ(b.size(), 1u);
    EXPECT_TRUE(b.contains(Point2D(1, 1)));
    EXPECT_FALSE(b.contains(Point2D(9, 9)));
    EXPECT_TRUE(a.empty());
}

TEST(PointSet, SelfMoveAndMoveFromEmpty) {
    PointSet a;
    a.add(Point2D(1, 1));
    PointSet& ref = a;
    a = std::move(ref);
    EXPECT_EQ(a.size(), 1u);
    EXPECT_TRUE(a.contains(Point2D(1, 1)));

    PointSet e;
    PointSet f(std::move(e));
    EXPECT_TRUE(e.empty());
    EXPECT_TRUE(f.empty());
}


TEST(PointSet, UnionIntersectionDifference) {
    PointSet a, b;
    a.add(Point2D(1, 1));
    a.add(Point2D(2, 2));
    a.add(Point2D(3, 3));
    b.add(Point2D(2, 2));
    b.add(Point2D(3, 3));
    b.add(Point2D(4, 4));

    PointSet u = a | b;
    EXPECT_EQ(u.size(), 4u);
    for (int i = 1; i <= 4; ++i) EXPECT_TRUE(u.contains(Point2D(i, i)));

    PointSet inter = a & b;
    EXPECT_EQ(inter.size(), 2u);
    EXPECT_TRUE(inter.contains(Point2D(2, 2)));
    EXPECT_TRUE(inter.contains(Point2D(3, 3)));

    PointSet d = a - b;
    EXPECT_EQ(d.size(), 1u);
    EXPECT_TRUE(d.contains(Point2D(1, 1)));


    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(b.size(), 3u);
}

TEST(PointSet, OperationsWithEmpty) {
    PointSet a, e;
    a.add(Point2D(1, 1));
    a.add(Point2D(2, 2));

    EXPECT_TRUE((a | e) == a);
    EXPECT_TRUE((e | a) == a);
    EXPECT_TRUE((a & e).empty());
    EXPECT_TRUE((a - e) == a);
    EXPECT_TRUE((e - a).empty());
}


TEST(PointSet, EqualityIgnoresOrder) {
    PointSet a, b;
    a.add(Point2D(1, 1));
    a.add(Point2D(2, 2));
    b.add(Point2D(2, 2));
    b.add(Point2D(1, 1));
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    b.add(Point2D(3, 3));
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);

    PointSet c;  
    c.add(Point2D(1, 1));
    c.add(Point2D(9, 9));
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}


TEST(PointSet, OutputFormat) {
    PointSet s;
    EXPECT_EQ(str(s), "{ }");
    s.add(Point2D(0, 0));
    EXPECT_EQ(str(s), "{ (0, 0) }");
    s.add(Point2D(1, 2));
    EXPECT_EQ(str(s), "{ (0, 0), (1, 2) }");
}


TEST(PointSet, Stress1000) {
    PointSet s;
    for (int i = 0; i < 1000; ++i) EXPECT_TRUE(s.add(Point2D(i, i * 2)));
    EXPECT_EQ(s.size(), 1000u);

    for (int i = 0; i < 1000; ++i) EXPECT_FALSE(s.add(Point2D(i, i * 2)));
    EXPECT_EQ(s.size(), 1000u);

    for (int i = 0; i < 1000; ++i) EXPECT_TRUE(s.contains(Point2D(i, i * 2)));

    for (int i = 0; i < 1000; i += 2) EXPECT_TRUE(s.remove(Point2D(i, i * 2)));
    EXPECT_EQ(s.size(), 500u);
}