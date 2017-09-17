#include <iostream>
#include "defaulted.h"
namespace
{
namespace test
{
        
int f1(int , fluent::Defaulted<int, 42> y, int )
{
    return y.get();
}

bool testDefaultValue()
{
    return f1(0, fluent::defaultValue, 1) == 42;
}

bool testValueProvided()
{
    return f1(0, 43, 1) == 43;
}

std::string f2(int , fluent::Defaulted<std::string> y, int )
{
    return y.get();
}

bool testDefaultConstructor()
{
    return f2(0, fluent::defaultValue, 1) == std::string();
}

bool testDefaultConstructorValueProvided()
{
    return f2(0, std::string("hello"), 1) == "hello";
}

struct GetDefaultAmount{ static double get(){ return 45.6; } };

double f3(int x, fluent::DefaultedF<double, GetDefaultAmount> y, int z)
{
    return y.get();
}

bool testDefaultedF()
{
    return f3(0, fluent::defaultValue, 1) == 45.6;
}

bool testDefaultedFValueProvided()
{
    return f3(0, 13.4, 1) == 13.4;
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

bool testDefaultedMakesACopy()
{
    int copyCount = 0;
    f4(CopyLogger(copyCount));
    return copyCount == 1;
}
    
void f5(fluent::Defaulted<CopyLogger const&>){}
    
bool testDefaultedConstRefMakesNoCopy()
{
    int copyCount = 0;
    f5(CopyLogger(copyCount));
    return copyCount == 0;
}

    
template <typename TestFunction>
bool launchTest(std::string const& testName, TestFunction testFunction)
{
    const bool success = testFunction();
    if (!success)
        std::cout << "Test - " << testName << ": FAILED\n";
    return success;
}

void launchTests()
{
    bool success = true;
    success &= launchTest("Default value", testDefaultValue);
    success &= launchTest("Value provided", testValueProvided);
    success &= launchTest("Default constructor", testDefaultConstructor);
    success &= launchTest("Default constructor value provided", testDefaultConstructorValueProvided);
    success &= launchTest("DefaultedF", testDefaultedF);
    success &= launchTest("DefaultedF value provided", testDefaultedFValueProvided);
    success &= launchTest("Defaulted makes a copy", testDefaultedMakesACopy);
    success &= launchTest("Defaulted const& makes no copy", testDefaultedConstRefMakesNoCopy);

    if (success)
        std::cout << "All tests PASSED\n";
}
}
}


int main()
{
    test::launchTests();
}
