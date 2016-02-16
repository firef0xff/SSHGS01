#include "switchtest.h"
#include <QJsonObject>
#include "../test_params_hydro.h"

namespace test
{

namespace hydro
{

SwitchTest::SwitchTest():
    test::hydro::Test( "Проверка переключения запорно-регулирующего\nэлемента пониженным напряжением", 7 ),
    ResultMinA(false),
    ResultMaxA(false),
    ResultMinB(false),
    ResultMaxB(false)
{}

bool SwitchTest::Run()
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;
    Start();
    Wait( mResults.op7_ok, mResults.op7_end );
    if ( IsStopped() )
        return false;

    ResultMinA = mResults.op7_min_ok_a &&! mResults.op7_min_no_a;
    ResultMaxA = mResults.op7_max_ok_a &&! mResults.op7_max_no_a;
    ResultMinB = mResults.op7_min_ok_b &&! mResults.op7_min_no_b;
    ResultMaxB = mResults.op7_max_ok_b &&! mResults.op7_max_no_b;

    ResultMinA_OFF = mResults.OP7_Min_D_YESa_off &&! mResults.OP7_Min_D_NOa_off;
    ResultMaxA_OFF = mResults.OP7_Max_D_YESa_off &&! mResults.OP7_Max_D_NOa_off;
    ResultMinB_OFF = mResults.OP7_Min_D_YESb_off &&! mResults.OP7_Min_D_NOb_off;
    ResultMaxB_OFF = mResults.OP7_Max_D_YESb_off &&! mResults.OP7_Max_D_NOb_off;

    OilTemp = round(mResults.T_oil*100)/100;

    return Success();
}

QJsonObject SwitchTest::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("ResultMinA", ResultMinA );
    obj.insert("ResultMaxA", ResultMaxA );
    obj.insert("ResultMinB", ResultMinB );
    obj.insert("ResultMaxB", ResultMaxB );

    obj.insert("ResultMinA_OFF", ResultMinA_OFF );
    obj.insert("ResultMaxA_OFF", ResultMaxA_OFF );
    obj.insert("ResultMinB_OFF", ResultMinB_OFF );
    obj.insert("ResultMaxB_OFF", ResultMaxB_OFF );

    return obj;

}
bool SwitchTest::Deserialize( QJsonObject const& obj )
{
    ResultMinA = obj.value("ResultMinA").toBool();
    ResultMaxA = obj.value("ResultMaxA").toBool();
    ResultMinB = obj.value("ResultMinB").toBool();
    ResultMaxB = obj.value("ResultMaxB").toBool();

    ResultMinA_OFF = obj.value("ResultMinA_OFF").toBool();
    ResultMaxA_OFF = obj.value("ResultMaxA_OFF").toBool();
    ResultMinB_OFF = obj.value("ResultMinB_OFF").toBool();
    ResultMaxB_OFF = obj.value("ResultMaxB_OFF").toBool();
    Test::Deserialize( obj );
    return true;
}

bool SwitchTest::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
                                                    QString const& value = "",
                                                    QColor const& color3 = Qt::black,
                                                    QString const& value2 = "")
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
        QPoint start_point2( start_point.x() + metrix.width(label), place.center().y() +metrix.height()/2);
        QPoint start_point3( start_point2.x() + metrix.width(value), place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color1 );
        painter.drawText( start_point, label );
        painter.setPen( color2 );
        painter.drawText( start_point2, value );
        painter.setPen( color3 );
        painter.drawText( start_point3, value2 );
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
        DrawRowCenter( rect, header_font, Qt::black, "6.Проверка переключения запорно-регулирующего" );
    }, 1.5 );
    res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "элемента пониженным напряжением" );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Данное испытание проводится аналогично с испытанием функционирования. " );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Разница заключается в том, что на распределитель подается пониженное" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "напряжение. Это пониженное напряжения =  « номинальное напряжения питания» –" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "«Допустимые отклонения напряжения питания»." );
    }, 1.5 );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, test::ToString(params->MinTestPressure()) + ", " + test::ToString(params->MaxWorkPressure()) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Расход при проведении испытаний, л/мин" ), Qt::black, test::ToString( params->DefaultExpenditure() ) );
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
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Распределитель ",
                                      Qt::red, (Success()? "переключается" : "не переключается"),
                                      Qt::black, " при допустимом пониженном напряжении управления."  );
    }, 3 );

    return res;
}
bool SwitchTest::Success() const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;
    return ResultMinA && ResultMaxA &&
            ResultMinA_OFF && ResultMaxA_OFF &&
            ( params->PosCount() == 2 ?
                  ResultMinB && ResultMaxB &&
                  ResultMinB_OFF && ResultMaxB_OFF
                : true );
}

}//namespace hydro

}//namespace test
