#include <iostream>
#include "defaulted.h"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

int f1(int , fluent::Defaulted<int, 42> y, int )
{
    return y.get();
}

TEST_CASE("Default value", "[Defaulted]")
{
    REQUIRE(f1(0, fluent::defaultValue, 1) == 42);
}

TEST_CASE("Value provided", "[Defaulted]")
{
    REQUIRE(f1(0, 43, 1) == 43);
}

std::string f2(int , fluent::Defaulted<std::string> y, int )
{
    return y.get();
}

TEST_CASE("Default constructor", "[Defaulted]")
{
    REQUIRE(f2(0, fluent::defaultValue, 1) == std::string());
}

TEST_CASE("Default constructor value provided", "[Defaulted]")
{
    REQUIRE(f2(0, std::string("hello"), 1) == "hello");
}

struct GetDefaultAmount{ static double get(){ return 45.6; } };

double f3(int x, fluent::DefaultedF<double, GetDefaultAmount> y, int z)
{
    return y.get();
}

TEST_CASE("DefaultedF Default value", "[DefaultedF]")
{
    REQUIRE(f3(0, fluent::defaultValue, 1) == 45.6);
}

TEST_CASE("DefaultedF Value provided", "[DefaultedF]")
{
    REQUIRE(f3(0, 13.4, 1) == 13.4);
}
    
class CopyLogger
{
public:
    CopyLogger(int& copyCount) : copyCount_(copyCount){}
    CopyLogger(CopyLogger const& other) : copyCount_(other.copyCount_){ ++copyCount_; }
private:
    int& copyCount_;
};
    
void f4(fluent::Defaulted<CopyLogger>){}

TEST_CASE("Defaulted makes a copy", "[Defaulted]")
{
    int copyCount = 0;
    f4(CopyLogger(copyCount));
    REQUIRE(copyCount == 1);
}
    
void f5(fluent::Defaulted<CopyLogger const&>){}
    
TEST_CASE("Defaulted const ref makes no copy", "[Defaulted]")
{
    int copyCount = 0;
    f5(CopyLogger(copyCount));
    REQUIRE(copyCount == 0);
}
