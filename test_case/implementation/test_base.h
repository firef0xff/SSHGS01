#pragma once
#include "../test.h"


namespace test
{

namespace hydro
{

class Test :public test::Test
{
public:
    Test( QString const& name, uint8_t id );
private:
    static uint8_t mTestsCount;
};

}//namespace hydro

namespace servo
{

class Test :public test::Test
{
public:
    Test( QString const& name, uint8_t id );
private:
    static uint8_t mTestsCount;
};

}//namespace servo

}//namespace test
