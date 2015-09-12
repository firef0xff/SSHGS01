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

}//namespace test

