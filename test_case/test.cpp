#include "test.h"
#include "test_case.h"

namespace test
{

Test::Test(TestCase *test_case, QString const& name, uint8_t number )
    : mName(name), mNumber(number), mCase(test_case),mStopMarker(nullptr)
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

QString const& Test::Name() const
{
    return mName;
}
uint8_t const& Test::Number() const
{
    return mNumber;
}

bool Test::Run( LogFunction to_log, bool& stop )
{
    Log = to_log;
    mStopMarker = &stop;

    bool res = Run();

    Log = LogFunction();
    mStopMarker = nullptr;
}

}//namespace test
