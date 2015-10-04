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

namespace control_board
{
class Test :public test::Test
{
public:
    Test( QString const& name, uint8_t id );
private:
    static uint8_t mTestsCount;
};
}//namespace control_board

namespace hydro_cylinder
{
class Test :public test::Test
{
public:
    Test( QString const& name, uint8_t id );
private:
    static uint8_t mTestsCount;
};
}//namespace hydro_cylinder


}//namespace test
