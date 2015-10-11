#include "test.h"
#include "test_case.h"

namespace test
{

Test::Test(TestCase *test_case, QString const& name, uint8_t number , uint8_t id)
    : mName(name), mNumber(number), mId(id), mCase(test_case),mStopMarker(nullptr),
      mDrawLine( 0 )
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
uint8_t const& Test::ID() const
{
    return mId;
}
bool Test::Run( LogFunction to_log, bool& stop )
{
    Log = to_log;
    mStopMarker = &stop;

    bool res = Run();

    Log = LogFunction();
    mStopMarker = nullptr;

    return res;
}

bool Test::IsStopped()
{
    return  mStopMarker && *mStopMarker;
}

void Test::ResetDrawLine()
{
    mDrawLine = 0;
}
bool Test::AllocatePlace( QRect& place, int height, QRect& source ) const
{
//    height = 1.25 * height;
    if ( source.height() < height)
        return false;

    place.setTop( source.top() );
    place.setLeft( source.left() );
    place.setHeight( height );
    place.setWidth( source.width() );

    source.setTop( place.bottom() );

    return true;
}
bool Test::DrawLine ( uint32_t& num, QRect& source, QFont font, DrawLineHandler do_draw, int custom_height ) const
{
    bool res = false;
    if ( mDrawLine > num )
        res = true;
    else if ( mDrawLine < num )
        res = false;
    else
    {
        QFontMetrics metrix( font );
        int height = custom_height ? custom_height : metrix.height();

        QRect place;
        if ( AllocatePlace( place, height, source ) )
        {
            do_draw( place );
            ++mDrawLine;
            res = true;
        }
        else
            res = false;
    }
    ++num;
    return res;
}

}//namespace test
