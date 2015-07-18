#include "test.h"
#include "test_case.h"

namespace test
{

Test::Test(TestCase *test_case, QString const& name, uint8_t number )
    : mName(name), mNumber(number), mCase(test_case)
{
    if (mCase)
        mCase -> Register( this );
}

Test::~Test()
{
    if (mCase)
        mCase -> UnRegister( this );
}

void Test::Free()
{
    mCase = nullptr;
}

QString const& Test::Name()
{
    return mName;
}
uint8_t const& Test::Number()
{
    return mNumber;
}

}//namespace test
