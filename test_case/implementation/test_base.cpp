#include "test_base.h"
#include "../tests.h"

namespace test
{

namespace hydro
{

uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::Test( &HydroTests, name, mTestsCount, id )
{
    ++mTestsCount;
}

void Test::Start()
{
    mCommand.N_Operation = mId;
    mCommand.Start_Oper = true;
    mCommand.Stop_Oper = false;
    mCommand.Nasos_M2 = false;
    mCommand.Write();
}
void Test::Wait( bool& work, bool& done)
{
    work = false;
    done = false;
    while( !done )
    {
        mResults.Read();
    }
}

}//namespace hydro

namespace servo
{

uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::Test( &ServoTests, name, mTestsCount, id )
{
    ++mTestsCount;
}
}//namespace servo

namespace control_board
{
uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::Test( &ControlBoardTests, name, mTestsCount, id )
{
    ++mTestsCount;
}
}//namespace control_board

namespace hydro_cylinder
{
uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::Test( &HydroCylinder, name, mTestsCount, id )
{
    ++mTestsCount;
}
}//namespace hydro_cylinder



}//namespace test

