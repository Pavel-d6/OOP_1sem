#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <utility>

#include "BodyCollection.hpp"
#include "mainClass.hpp"
#include "moon.hpp"
#include "planet.hpp"
#include "star.hpp"

namespace {

constexpr double kG = 6.674e-11;

double expectedG(double M, double R) { return kG * M / (R * R); }

std::string toString(const CelestialBody& b) {
    std::ostringstream os;
    os << b;
    return os.str();
}

bool contains(const std::string& s, const std::string& part) {
    return s.find(part) != std::string::npos;
}

class Probe : public CelestialBody {
    int* destroyed_;

public:
    explicit Probe(int* destroyed) : CelestialBody(1.0, 1.0), destroyed_(destroyed) {}
    ~Probe() override { ++*destroyed_; }
    void print(std::ostream&) const override {}
    void read(std::istream&) override {}
    CelestialBody* clone() const override { return new Probe(*this); }
};

Star sun() { return Star(1.989e30, 6.957e8, 5772.0); }
Planet earth() { return Planet(5.97e24, 6.371e6, 288.0); }
Moon moonOfEarth() { return Moon(7.35e22, 1.737e6, 27.3, 27.3); }

}

TEST(Metric, StarFollowsFormula) {
    Star s(2.0e30, 1.0e9, 5000.0);
    EXPECT_DOUBLE_EQ(s.metric(), expectedG(2.0e30, 1.0e9));
}

TEST(Metric, PlanetFollowsFormula) {
    Planet p(6.0e24, 6.4e6, 300.0);
    EXPECT_DOUBLE_EQ(p.metric(), expectedG(6.0e24, 6.4e6));
}

TEST(Metric, MoonFollowsFormula) {
    Moon m(7.0e22, 1.7e6, 27.0, 27.0);
    EXPECT_DOUBLE_EQ(m.metric(), expectedG(7.0e22, 1.7e6));
}

TEST(Metric, RealWorldSanityValues) {
    EXPECT_NEAR(sun().metric(), 274.0, 1.0);
    EXPECT_NEAR(earth().metric(), 9.82, 0.02);
    EXPECT_NEAR(moonOfEarth().metric(), 1.63, 0.02);
}

TEST(StarTest, StellarClassByTemperature) {
    struct Case { double T; char cls; };
    const Case cases[] = {
        {50000, 'O'}, {30000, 'O'}, {29999, 'B'}, {10000, 'B'}, {9999, 'A'},
        {7500, 'A'},  {7499, 'F'},  {6000, 'F'},  {5999, 'G'},  {5772, 'G'},
        {5200, 'G'},  {5199, 'K'},  {3700, 'K'},  {3699, 'M'},  {2500, 'M'},
    };
    for (const Case& c : cases) {
        EXPECT_EQ(Star(1.0, 1.0, c.T).stellarClass(), c.cls) << "T=" << c.T;
    }
}

TEST(PlanetTest, HabitableEarthLike) {
    EXPECT_TRUE(earth().isHabitable());
}

TEST(PlanetTest, NotHabitableWhenTooHotOrCold) {
    EXPECT_FALSE(Planet(5.97e24, 6.371e6, 400.0).isHabitable());
    EXPECT_FALSE(Planet(5.97e24, 6.371e6, 100.0).isHabitable());
}

TEST(PlanetTest, TemperatureWindowIsStrict) {
    EXPECT_TRUE(Planet(5.97e24, 6.371e6, 349.0).isHabitable());
    EXPECT_FALSE(Planet(5.97e24, 6.371e6, 350.0).isHabitable());
    EXPECT_TRUE(Planet(5.97e24, 6.371e6, 231.0).isHabitable());
    EXPECT_FALSE(Planet(5.97e24, 6.371e6, 230.0).isHabitable());
}

TEST(PlanetTest, NotHabitableWhenGravityOutOfRange) {
    EXPECT_FALSE(Planet(1.898e27, 6.99e7, 290.0).isHabitable());
    EXPECT_FALSE(Planet(7.35e22, 1.737e6, 290.0).isHabitable());
}

TEST(MoonTest, TidallyLockedWhenPeriodsEqual) {
    EXPECT_TRUE(moonOfEarth().tidallyLocked());
}

TEST(MoonTest, NotLockedWhenPeriodsDiffer) {
    EXPECT_FALSE(Moon(7.35e22, 1.737e6, 27.3, 1.0).tidallyLocked());
}

TEST(MoonTest, LockedToleranceIsRelative) {
    EXPECT_TRUE(Moon(7.35e22, 1.737e6, 1.0e6, 1.0e6 + 0.5).tidallyLocked());
    EXPECT_FALSE(Moon(7.35e22, 1.737e6, 1.0e6, 1.0e6 + 2.0).tidallyLocked());
}


TEST(Cast, CorrectTypeGivesNonNull) {
    Star s = sun();
    Planet p = earth();
    Moon m = moonOfEarth();
    CelestialBody* bs = &s;
    CelestialBody* bp = &p;
    CelestialBody* bm = &m;
    EXPECT_NE(dynamic_cast<Star*>(bs), nullptr);
    EXPECT_NE(dynamic_cast<Planet*>(bp), nullptr);
    EXPECT_NE(dynamic_cast<Moon*>(bm), nullptr);
}

TEST(Cast, WrongTypeGivesNullptr) {
    Star s = sun();
    Planet p = earth();
    Moon m = moonOfEarth();
    CelestialBody* bs = &s;
    CelestialBody* bp = &p;
    CelestialBody* bm = &m;
    EXPECT_EQ(dynamic_cast<Planet*>(bs), nullptr);
    EXPECT_EQ(dynamic_cast<Moon*>(bs), nullptr);
    EXPECT_EQ(dynamic_cast<Star*>(bp), nullptr);
    EXPECT_EQ(dynamic_cast<Moon*>(bp), nullptr);
    EXPECT_EQ(dynamic_cast<Star*>(bm), nullptr);
    EXPECT_EQ(dynamic_cast<Planet*>(bm), nullptr);
}

TEST(Cast, SpecificMethodThroughBasePointer) {
    BodyCollection c;
    c.add(new Star(sun()));
    c.add(new Moon(moonOfEarth()));
    const Star* s = dynamic_cast<const Star*>(&c[0]);
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->stellarClass(), 'G');
    const Moon* m = dynamic_cast<const Moon*>(&c[1]);
    ASSERT_NE(m, nullptr);
    EXPECT_TRUE(m->tidallyLocked());
}

TEST(Collection, TotalMetricOfSeveralObjects) {
    BodyCollection c;
    c.add(new Star(1.0e30, 1.0e9, 5000.0));
    c.add(new Planet(2.0e24, 5.0e6, 300.0));
    c.add(new Moon(3.0e22, 1.5e6, 10.0, 10.0));
    EXPECT_EQ(c.size(), 3u);
    const double expected = expectedG(1.0e30, 1.0e9) + expectedG(2.0e24, 5.0e6) + expectedG(3.0e22, 1.5e6);
    EXPECT_NEAR(c.totalMetric(), expected, 1e-9);
}

TEST(Collection, CountByType) {
    BodyCollection c;
    c.add(new Star(sun()));
    c.add(new Star(sun()));
    c.add(new Planet(earth()));
    c.add(new Moon(moonOfEarth()));
    c.add(new Moon(moonOfEarth()));
    c.add(new Moon(moonOfEarth()));
    const TypeCounts n = c.countByType();
    EXPECT_EQ(n.stars, 2u);
    EXPECT_EQ(n.planets, 1u);
    EXPECT_EQ(n.moons, 3u);
}

TEST(Collection, GrowsBeyondInitialCapacity) {
    BodyCollection c;
    for (int i = 0; i < 100; ++i) c.add(new Planet(earth()));
    EXPECT_EQ(c.size(), 100u);
    EXPECT_NEAR(c.totalMetric(), 100 * earth().metric(), 1e-6);
}

TEST(Collection, CopyIsDeepAndKeepsDynamicTypes) {
    BodyCollection a;
    a.add(new Star(sun()));
    a.add(new Planet(earth()));
    a.add(new Moon(moonOfEarth()));
    BodyCollection b(a);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_NE(&a[0], &b[0]);
    EXPECT_NE(dynamic_cast<const Star*>(&b[0]), nullptr);
    EXPECT_NE(dynamic_cast<const Planet*>(&b[1]), nullptr);
    EXPECT_NE(dynamic_cast<const Moon*>(&b[2]), nullptr);
    a.add(new Star(sun()));
    EXPECT_EQ(b.size(), 3u);
}

TEST(Collection, CopyAssignmentAndSelfAssignment) {
    BodyCollection a;
    a.add(new Star(sun()));
    BodyCollection b;
    b.add(new Planet(earth()));
    b.add(new Planet(earth()));
    b = a;
    EXPECT_EQ(b.size(), 1u);
    EXPECT_NE(dynamic_cast<const Star*>(&b[0]), nullptr);
    BodyCollection& same = b;
    b = same;
    EXPECT_EQ(b.size(), 1u);
}

TEST(Collection, MoveLeavesSourceEmpty) {
    BodyCollection a;
    a.add(new Star(sun()));
    a.add(new Planet(earth()));
    BodyCollection b(std::move(a));
    EXPECT_EQ(b.size(), 2u);
    EXPECT_EQ(a.size(), 0u);
    BodyCollection c;
    c = std::move(b);
    EXPECT_EQ(c.size(), 2u);
    EXPECT_EQ(b.size(), 0u);
}


TEST(Stream, OutputContainsClassNameAndFields) {
    const std::string s = toString(Star(1.0, 2.0, 5772.0));
    EXPECT_TRUE(contains(s, "Star:"));
    EXPECT_TRUE(contains(s, "M=1"));
    EXPECT_TRUE(contains(s, "R=2"));
    EXPECT_TRUE(contains(s, "T=5772 K"));

    const std::string p = toString(Planet(3.0, 4.0, 288.0));
    EXPECT_TRUE(contains(p, "Planet:"));
    EXPECT_TRUE(contains(p, "M=3"));
    EXPECT_TRUE(contains(p, "T=288 K"));

    const std::string m = toString(Moon(5.0, 6.0, 27.0, 28.0));
    EXPECT_TRUE(contains(m, "Moon:"));
    EXPECT_TRUE(contains(m, "T_orbit=27"));
    EXPECT_TRUE(contains(m, "T_rot=28"));
}

TEST(Stream, OutputIsPolymorphicThroughBaseReference) {
    Moon m = moonOfEarth();
    const CelestialBody& ref = m;
    EXPECT_TRUE(contains(toString(ref), "Moon:"));
}


TEST(Stream, InputStarFromString) {
    Star s;
    std::istringstream in("1.989e30 6.957e8 5772");
    in >> s;
    EXPECT_FALSE(in.fail());
    EXPECT_DOUBLE_EQ(s.metric(), expectedG(1.989e30, 6.957e8));
    EXPECT_EQ(s.stellarClass(), 'G');
}

TEST(Stream, InputPlanetAndMoonFromString) {
    Planet p;
    std::istringstream inP("5.97e24 6.371e6 288");
    inP >> p;
    EXPECT_DOUBLE_EQ(p.metric(), expectedG(5.97e24, 6.371e6));
    EXPECT_TRUE(p.isHabitable());

    Moon m;
    std::istringstream inM("7.35e22 1.737e6 27.3 27.3");
    inM >> m;
    EXPECT_DOUBLE_EQ(m.metric(), expectedG(7.35e22, 1.737e6));
    EXPECT_TRUE(m.tidallyLocked());
}

TEST(Stream, InputWorksThroughBaseReference) {
    Star s;
    CelestialBody& ref = s;
    std::istringstream in("2e30 7e8 6000");
    in >> ref;
    EXPECT_EQ(s.stellarClass(), 'F');
}

TEST(Stream, BadInputSetsFailbitAndKeepsObject) {
    Star s = sun();
    const double before = s.metric();

    std::istringstream garbage("abc def ghi");
    garbage >> s;
    EXPECT_TRUE(garbage.fail());
    EXPECT_DOUBLE_EQ(s.metric(), before);

    std::istringstream negative("-1 5 5000");
    negative >> s;
    EXPECT_TRUE(negative.fail());
    EXPECT_DOUBLE_EQ(s.metric(), before);

    std::istringstream truncated("1e30 1e9");
    truncated >> s;
    EXPECT_TRUE(truncated.fail());
    EXPECT_DOUBLE_EQ(s.metric(), before);
}

TEST(Stream, MoonRejectsNonPositivePeriods) {
    Moon m = moonOfEarth();
    const double before = m.metric();
    std::istringstream in("7.35e22 1.737e6 0 27.3");
    in >> m;
    EXPECT_TRUE(in.fail());
    EXPECT_DOUBLE_EQ(m.metric(), before);
    EXPECT_TRUE(m.tidallyLocked());
}


TEST(Equality, EqualByMetricEvenForDifferentTypes) {
    Planet p(5.97e24, 6.371e6, 288.0);
    Star s(5.97e24, 6.371e6, 5000.0);
    EXPECT_TRUE(p == s);
}

TEST(Equality, NotEqualWhenMetricsDiffer) {
    EXPECT_FALSE(earth() == moonOfEarth());
    EXPECT_FALSE(sun() == earth());
}


TEST(Conversion, OperatorDoubleReturnsMetric) {
    Planet p = earth();
    double g = p;
    EXPECT_DOUBLE_EQ(g, p.metric());
    EXPECT_DOUBLE_EQ(static_cast<double>(sun()), sun().metric());
}

TEST(Conversion, WorksThroughBaseReference) {
    Moon m = moonOfEarth();
    const CelestialBody& ref = m;
    EXPECT_DOUBLE_EQ(static_cast<double>(ref), m.metric());
}


TEST(Boundary, NonPositiveMassOrRadiusThrows) {
    EXPECT_THROW(Star(0.0, 1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(Star(-1.0, 1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(Planet(1.0, 0.0, 1.0), std::invalid_argument);
    EXPECT_THROW(Planet(1.0, -1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(Moon(0.0, 1.0, 1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(Moon(1.0, -5.0, 1.0, 1.0), std::invalid_argument);
}

TEST(Boundary, NonPositiveMoonPeriodsThrow) {
    EXPECT_THROW(Moon(1.0, 1.0, 0.0, 1.0), std::invalid_argument);
    EXPECT_THROW(Moon(1.0, 1.0, 1.0, 0.0), std::invalid_argument);
    EXPECT_THROW(Moon(1.0, 1.0, -1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(Moon(1.0, 1.0, 1.0, -1.0), std::invalid_argument);
}

TEST(Boundary, DefaultConstructorsGiveValidObjects) {
    Star s;
    Planet p;
    Moon m;
    EXPECT_DOUBLE_EQ(s.metric(), kG);  
    EXPECT_DOUBLE_EQ(p.metric(), kG);
    EXPECT_DOUBLE_EQ(m.metric(), kG);
    EXPECT_TRUE(m.tidallyLocked());
}

TEST(Boundary, TemperatureIsNotValidatedByDesign) {
    EXPECT_NO_THROW(Planet(1.0, 1.0, -5.0));
    EXPECT_FALSE(Planet(5.97e24, 6.371e6, -5.0).isHabitable());
    EXPECT_EQ(Star(1.0, 1.0, -5.0).stellarClass(), 'M');
}

TEST(Boundary, EmptyCollection) {
    BodyCollection c;
    EXPECT_EQ(c.size(), 0u);
    EXPECT_DOUBLE_EQ(c.totalMetric(), 0.0);
    const TypeCounts n = c.countByType();
    EXPECT_EQ(n.stars, 0u);
    EXPECT_EQ(n.planets, 0u);
    EXPECT_EQ(n.moons, 0u);
    EXPECT_THROW(c[0], std::out_of_range);
    BodyCollection copy(c);
    EXPECT_EQ(copy.size(), 0u);
}

TEST(Boundary, CollectionRejectsNullAndBadIndex) {
    BodyCollection c;
    EXPECT_THROW(c.add(nullptr), std::invalid_argument);
    c.add(new Star(sun()));
    EXPECT_NO_THROW(c[0]);
    EXPECT_THROW(c[1], std::out_of_range);
}


TEST(Memory, VirtualDestructorRunsThroughBasePointer) {
    int destroyed = 0;
    CelestialBody* b = new Probe(&destroyed);
    delete b;
    EXPECT_EQ(destroyed, 1);
}

TEST(Memory, CollectionDeletesEveryObjectExactlyOnce) {
    int destroyed = 0;
    {
        BodyCollection c;
        c.add(new Probe(&destroyed));
        c.add(new Probe(&destroyed));
        c.add(new Probe(&destroyed));
        EXPECT_EQ(destroyed, 0);
    }
    EXPECT_EQ(destroyed, 3);
}

TEST(Memory, CopyAndMoveDoNotDoubleDelete) {
    int destroyed = 0;
    {
        BodyCollection a;
        a.add(new Probe(&destroyed));
        a.add(new Probe(&destroyed));
        BodyCollection copy(a);              
        BodyCollection moved(std::move(a)); 
    }
    EXPECT_EQ(destroyed, 4);  
}