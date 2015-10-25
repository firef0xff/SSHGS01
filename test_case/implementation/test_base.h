#pragma once
#include "../test.h"

#include "../../cpu/db31.h"
#include "../../cpu/db32.h"

namespace test
{

namespace hydro
{

class Test :public test::Test
{
public:
    Test( QString const& name, uint8_t id );
protected:
    void Start();
    void Wait( bool& work, bool& done);
    cpu::data::DB31 mCommand;
    cpu::data::DB32 mResults;

    double OilTemp;
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
