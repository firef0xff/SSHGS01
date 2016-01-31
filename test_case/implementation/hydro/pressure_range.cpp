#include "pressure_range.h"
#include <QJsonObject>
#include "../test_params_hydro.h"

namespace test
{

namespace hydro
{

PressureRange::PressureRange():
    test::hydro::Test( "Проверка диапазона давления управления", 6 ),
    ResultMinMinA(false),
    ResultMinMaxA(false),
    ResultMinMinB(false),
    ResultMinMaxB(false),
    ResultMaxMinA(false),
    ResultMaxMaxA(false),
    ResultMaxMinB(false),
    ResultMaxMaxB(false)
{}

bool PressureRange::Run()
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;

    Start();
    Wait( mResults.op6_ok, mResults.op6_end );
    if ( IsStopped() )
        return false;

    ResultMinMinA = mResults.op6_min_min_ok_a && !mResults.op6_min_min_no_a;
    ResultMinMaxA = mResults.op6_min_max_ok_a && !mResults.op6_min_max_no_a;
    ResultMinMinB = mResults.op6_min_min_ok_b && !mResults.op6_min_min_no_b;
    ResultMinMaxB = mResults.op6_min_max_ok_b && !mResults.op6_min_max_no_b;
    ResultMaxMinA = mResults.op6_max_min_ok_a && !mResults.op6_max_min_no_b;
    ResultMaxMaxA = mResults.op6_max_max_ok_a && !mResults.op6_max_max_no_a;
    ResultMaxMinB = mResults.op6_max_min_ok_b && !mResults.op6_mex_min_no_b;
    ResultMaxMaxB = mResults.op6_max_max_ok_b && !mResults.op6_max_max_no_b;

    ResultMinMinA_OFF = mResults.OP6_MinD_MinUpr_YESa_off && !mResults.OP6_MinD_MinUpr_NOa_off;
    ResultMinMaxA_OFF = mResults.OP6_MinD_MaxUpr_YESa_off && !mResults.OP6_MinD_MaxUpr_NOa_off;
    ResultMinMinB_OFF = mResults.OP6_MinD_MinUpr_YESb_off && !mResults.OP6_MinD_MinUpr_NOb_off;
    ResultMinMaxB_OFF = mResults.OP6_MinD_MaxUpr_YESb_off && !mResults.OP6_MinD_MaxUpr_NOb_off;
    ResultMaxMinA_OFF = mResults.OP6_MaxD_MinUpr_YESa_off && !mResults.OP6_MaxD_MinUpr_NOa_off;
    ResultMaxMaxA_OFF = mResults.OP6_MaxD_MaxUpr_YESa_off && !mResults.OP6_MaxD_MaxUpr_NOa_off;
    ResultMaxMinB_OFF = mResults.OP6_MaxD_MinUpr_YESb_off && !mResults.OP6_MaxD_MinUpr_NOb_off;
    ResultMaxMaxB_OFF = mResults.OP6_MaxD_MaxUpr_YESb_off && !mResults.OP6_MaxD_MaxUpr_NOb_off;

    OilTemp = round(mResults.T_oil*100)/100;

    return Success();
}

QJsonObject PressureRange::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("ResultMinMinA", ResultMinMinA );
    obj.insert("ResultMinMaxA", ResultMinMaxA );
    obj.insert("ResultMinMinB", ResultMinMinB );
    obj.insert("ResultMinMaxB", ResultMinMaxB );

    obj.insert("ResultMaxMinA", ResultMaxMinA );
    obj.insert("ResultMaxMaxA", ResultMaxMaxA );
    obj.insert("ResultMaxMinB", ResultMaxMinB );
    obj.insert("ResultMaxMaxB", ResultMaxMaxB );

    obj.insert("ResultMinMinA_OFF", ResultMinMinA_OFF );
    obj.insert("ResultMinMaxA_OFF", ResultMinMaxA_OFF );
    obj.insert("ResultMinMinB_OFF", ResultMinMinB_OFF );
    obj.insert("ResultMinMaxB_OFF", ResultMinMaxB_OFF );

    obj.insert("ResultMaxMinA_OFF", ResultMaxMinA_OFF );
    obj.insert("ResultMaxMaxA_OFF", ResultMaxMaxA_OFF );
    obj.insert("ResultMaxMinB_OFF", ResultMaxMinB_OFF );
    obj.insert("ResultMaxMaxB_OFF", ResultMaxMaxB_OFF );


    return obj;
}
bool PressureRange::Deserialize( QJsonObject const& obj )
{
    ResultMinMinA = obj.value("ResultMinMinA").toBool();
    ResultMinMaxA = obj.value("ResultMinMaxA").toBool();
    ResultMinMinB = obj.value("ResultMinMinB").toBool();
    ResultMinMaxB = obj.value("ResultMinMaxB").toBool();

    ResultMaxMinA = obj.value("ResultMaxMinA").toBool();
    ResultMaxMaxA = obj.value("ResultMaxMaxA").toBool();
    ResultMaxMinB = obj.value("ResultMaxMinB").toBool();
    ResultMaxMaxB = obj.value("ResultMaxMaxB").toBool();

    ResultMinMinA_OFF = obj.value("ResultMinMinA_OFF").toBool();
    ResultMinMaxA_OFF = obj.value("ResultMinMaxA_OFF").toBool();
    ResultMinMinB_OFF = obj.value("ResultMinMinB_OFF").toBool();
    ResultMinMaxB_OFF = obj.value("ResultMinMaxB_OFF").toBool();

    ResultMaxMinA_OFF = obj.value("ResultMaxMinA_OFF").toBool();
    ResultMaxMaxA_OFF = obj.value("ResultMaxMaxA_OFF").toBool();
    ResultMaxMinB_OFF = obj.value("ResultMaxMinB_OFF").toBool();
    ResultMaxMaxB_OFF = obj.value("ResultMaxMaxB_OFF").toBool();
    Test::Deserialize( obj );
    return true;
}

bool PressureRange::Draw( QPainter& painter, QRect &free_rect, QString  const& ) const
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
        DrawRowCenter( rect, header_font, Qt::black, "7.Проверка диапазона давления управления" );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Данное испытание предназначено для аппаратов с пилотным гидравлическим" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "управлением." );
    }, 1.5 );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Диапазон давления при проведении испытаний, бар"), Qt::red, test::ToString(params->MinTestPressure()) + " - " + test::ToString(params->MaxWorkPressure()) );
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
    }, 3 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Распределитель корректно ",
                                      Qt::red, (Success()? "переключается" : "не переключается"),
                                      Qt::black, " при всех  допустимых значениях давления"  );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "управления." );
    }, 1.5 );

    return res;
}
bool PressureRange::Success() const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;
    return ResultMinMinA && ResultMinMaxA && ResultMaxMinA && ResultMaxMaxA &&
           ResultMinMinA_OFF && ResultMinMaxA_OFF && ResultMaxMinA_OFF && ResultMaxMaxA_OFF &&
            ( params->ReelCount() == 2 ?
                  ResultMinMinB && ResultMinMaxB && ResultMaxMinB && ResultMaxMaxB &&
                  ResultMinMinB_OFF && ResultMinMaxB_OFF && ResultMaxMinB_OFF && ResultMaxMaxB_OFF
                : true );
}
}//namespace hydro

}//namespace test
