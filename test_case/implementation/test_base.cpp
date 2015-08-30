#include "test_base.h"
#include "../tests.h"

namespace test
{

namespace hydro
{

uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name ):
    test::Test( &HydroTests, name, mTestsCount )
{
    ++mTestsCount;
}


}//namespace hydro

namespace servo
{

uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name ):
    test::Test( &ServoTests, name, mTestsCount )
{
    ++mTestsCount;
}
}//namespace servo

}//namespace test

