#include "max_expenditure.h"
#include <QJsonObject>
#include "../test_params_hydro.h"

namespace test
{

namespace hydro
{

MaxExpenditureTest::MaxExpenditureTest():
    test::hydro::Test( "Проверка максимального расхода", 5 ),
    ResultA(false),
    ResultB(false)
{}

bool MaxExpenditureTest::Run()
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;

    Start();
    Wait( mResults.op5_ok, mResults.op5_end );
    if ( IsStopped() )
        return false;

    ResultA = mResults.op5_ok_a && !mResults.op5_no_a;
    ResultB = mResults.op5_ok_b && !mResults.op5_no_b;
    ExpenditureA = mResults.op5_pa;
    ExpenditureB = mResults.op5_pb;

    OilTemp = round(mResults.T_oil*100)/100;

    return Success();
}

QJsonObject MaxExpenditureTest::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("ResultA", ResultA );
    obj.insert("ResultB", ResultB );
    obj.insert("ExpenditureA", ExpenditureA );
    obj.insert("ExpenditureB", ExpenditureB );

    return obj;
}
bool MaxExpenditureTest::Deserialize( QJsonObject const& obj )
{
    ResultA = obj.value("ResultA").toBool();
    ResultB = obj.value("ResultB").toBool();
    ExpenditureA = obj.value("ExpenditureA").toDouble();
    ExpenditureB = obj.value("ExpenditureB").toDouble();

    Test::Deserialize( obj );
    return true;
}

bool MaxExpenditureTest::Draw(QPainter& painter, QRect &free_rect , const QString &) const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return true;

    QFont header_font = painter.font();
    header_font.setFamily("Arial");
    header_font.setPointSize( 14 );
    QFont result_font = header_font;
    result_font.setUnderline(true);
    QFont text_font = header_font;
    text_font.setPointSize( 12 );


    auto DrawRowCenter = [ &painter, &free_rect ](QRect const& place, QFont const& font, QColor const& color, QString const& text )
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.center().x() - metrix.width( text ) / 2, place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color );
        painter.drawText( start_point, text );
        painter.restore();
    };
    auto DrawRowLeft = [ &painter, &free_rect ](    QRect const& place,
                                                    QFont const& font,
                                                    QColor const& color1,
                                                    QString const& label,
                                                    QColor const& color2 = Qt::black,
                                                    QString const& value = "")
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
    };

    QFontMetrics m(text_font);
    int width = m.width("123456789012345678901234567890123456789012345");
    char symbol = '.';
    auto FillToSize = [ width, &m, symbol ]( QString text )
    {
        while( m.width( text + symbol ) < width )
            text += symbol;
        return text + " ";
    };


    uint32_t num = 0;
    bool res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "4."+mName );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Данное испытание предназначено для проверки максимальной пропускной" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "способности гидроаппарата при протекании жидкости  из канала Р в канал А или В," );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "и затем сравнивание её с исходными значениями." );
    }, 1.5 );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, test::ToString( params->MaxWorkPressure()) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Длительность испытания, сек"), Qt::red, test::ToString( TestingTime ) );
    }, 2 );


    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &result_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, result_font, Qt::black, "РЕЗУЛЬТАТ:" );
    }, 3 );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font, &FillToSize ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Максимальный расход Р-->A , л/мин"), Qt::red, test::ToString( ExpenditureA ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font, &FillToSize ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Максимальный расход Р-->В , л/мин"), Qt::red, test::ToString( ExpenditureB ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Испытание ", Qt::red, Success()? "пройдено" : "не пройдено" );
    }, 2 );
    return res;
}

bool MaxExpenditureTest::Success() const
{
    return ResultA && ResultB;
}
}//namespace hydro

}//namespace test
