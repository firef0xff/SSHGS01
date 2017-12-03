#include "test.h"
#include "test_case.h"

namespace test
{

Test::Test(TestCase *test_case, QString const& name, uint8_t number , uint8_t id)
    : mName(name), mId(id), mNumber(number), mCase(test_case),mStopMarker(nullptr),
      mDrawLine( 0 ), mDisabled(false)
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

bool Test::Disabled() const
{
    return mDisabled;
}
void Test::Disabled( bool val )
{
    mDisabled = val;
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
bool Test::Run(LaunchFunction to_launch, LogFunction to_log, bool& stop )
{
    Log = to_log;
    Launcher = to_launch;
    mStopMarker = &stop;

    bool res = Run();

    Log = LogFunction();
    Launcher = LaunchFunction();
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
bool Test::Success() const
{
    return false;
}
bool AllocatePlace( QRect& place, int height, QRect& source )
{
    if ( source.height() < height)
        return false;

    place.setTop( source.top() );
    place.setLeft( source.left() );
    place.setHeight( height );
    place.setWidth( source.width() );

    source.setTop( place.bottom() );

    return true;
}
bool Test::DrawLine ( uint32_t& num, QRect& source, QFont font, DrawLineHandler do_draw, double space, int custom_height ) const
{
    bool res = false;
    if ( mDrawLine > num )
        res = true;
    else if ( mDrawLine < num )
        res = false;
    else
    {
        QFontMetrics metrix( font );
        int height = custom_height ? custom_height : metrix.height() * space;

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


DrawHelper::DrawHelper( QPainter& p, QRect& r):
    painter(p),
    free_rect(r)
{

}
void DrawHelper::DrawRowCenter( QRect const& place, QFont const& font, QColor const& color, QString const& text )
{
    painter.save();
    QFontMetrics metrix( font );
    QPoint start_point( place.center().x() - metrix.width( text ) / 2, place.center().y() +metrix.height()/2);
    painter.setFont( font );
    painter.setPen( color );
    painter.drawText( start_point, text );
    painter.restore();
}

void DrawHelper::DrawRowLeft( QRect const& place,
                  QFont const& font,
                  QColor const& color1,
                  QString const& label)
{
    painter.save();
    QFontMetrics metrix( font );
    QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
    painter.setFont( font );
    painter.setPen( color1 );
    painter.drawText( start_point, label );
    painter.restore();
}

void DrawHelper::DrawRowLeft( QRect const& place,
                  QFont const& font,
                  QColor const& color1,
                  QString const& label,
                  QColor const& color2,
                  QString const& value )
{
    painter.save();
    QFontMetrics metrix( font );
    QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
    QPoint start_point2( place.left() + metrix.width(label), place.center().y() +metrix.height()/2);
    painter.setFont( font );
    painter.setPen( color1 );
    painter.drawText( start_point, label );
    painter.setPen( color2 );
    painter.drawText( start_point2, value );
    painter.restore();
}

void DrawHelper::DrawRowLeft( QRect const& place,
                              QFont const& font,
                              QColor const& color1,
                              QString const& label,
                              QColor const& color2,
                              QString const& value,
                              QColor const& color3,
                              QString const& value3 )
{
    painter.save();
    QFontMetrics metrix( font );
    QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
    QPoint start_point2( place.left() + metrix.width(label), place.center().y() +metrix.height()/2);
    QPoint start_point3( place.left() + metrix.width(label)+ metrix.width(value), place.center().y() +metrix.height()/2);
    painter.setFont( font );
    painter.setPen( color1 );
    painter.drawText( start_point, label );
    painter.setPen( color2 );
    painter.drawText( start_point2, value );
    painter.setPen( color3 );
    painter.drawText( start_point3, value3 );
    painter.restore();
}
}//namespace test
