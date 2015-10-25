#include "pressure_range.h"
#include <QJsonObject>
#include "../test_params_hydro.h"

namespace test
{

namespace hydro
{

PressureRange::PressureRange():
    test::hydro::Test( "Проверка диапазона давления управления\n(для направляющей гидроаппаратуры с электрогидравлическим управлением)", 7 ),
    ResultMinA(false),
    ResultMaxA(false),
    ResultMinB(false),
    ResultMaxB(false)
{}

bool PressureRange::Run()
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;
    Start();
    Wait( mResults.OP7_Work, mResults.OP7_End );

    ResultMinA = mResults.OP7_Min_D_YESa &&! mResults.OP7_Min_D_NOa;
    ResultMaxA = mResults.OP7_Max_D_YESa &&! mResults.OP7_Max_D_NOa;

    ResultMinB = mResults.OP7_Min_D_YESb &&! mResults.OP7_Min_D_NOb;
    ResultMaxB = mResults.OP7_Max_D_YESb &&! mResults.OP7_Max_D_NOb;

    return ResultMinA && ResultMaxA && ( params->ReelCount() == 2 ? ResultMinB && ResultMaxB : true );
}

QJsonObject PressureRange::Serialise() const
{
    QJsonObject obj;
    obj.insert("ResultMinA", ResultMinA );
    obj.insert("ResultMaxA", ResultMaxA );
    obj.insert("ResultMinB", ResultMinB );
    obj.insert("ResultMaxB", ResultMaxB );

    return obj;
}
bool PressureRange::Deserialize( QJsonObject const& obj )
{
    ResultMinA = obj.value("ResultMinA").toBool();
    ResultMaxA = obj.value("ResultMaxA").toBool();
    ResultMinB = obj.value("ResultMinB").toBool();
    ResultMaxB = obj.value("ResultMaxB").toBool();

    return true;
}

bool PressureRange::Draw( QPainter& painter, QRect &free_rect ) const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return true;

    QFont header_font = painter.font();
    QFont text_font = painter.font();
    header_font.setPointSize( 14 );
    text_font.setPointSize( 12 );

    QFontMetrics head_metrix( header_font );

    uint32_t num = 0;

    bool res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &head_metrix, &header_font ]( QRect const& rect )
    {
        QString name = "Проверка диапазона давления управления";
        QPoint start_point( rect.center().x() - head_metrix.width( name ) / 2, rect.bottom() );
        painter.setFont( header_font );
        painter.drawText( start_point, name );
    });

    painter.setFont( text_font );
    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, params ]( QRect const& rect )
    {
        QString s = "Позиция А: ";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, params ]( QRect const& rect )
    {
        QString s = "Распределитель ";
        s += ResultMinA&&ResultMaxA ? "" : "не";
        s += "корректно переключается при всех допустимых значениях давления";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, params ]( QRect const& rect )
    {
        QString s = "управления( от ";
        s += QString::number( params->MinControlPressure() ) + " до " + QString::number( params->MaxControlPressure() ) + " Бар )";
        painter.drawText( rect, s );
    });

    if ( params->ReelCount() == 2 )
    {
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter, params ]( QRect const& rect )
        {
            QString s = "Позиция Б: ";
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter, params ]( QRect const& rect )
        {
            QString s = "Распределитель ";
            s += ResultMinB&&ResultMaxB ? "" : "не";
            s += "корректно переключается при всех допустимых значениях давления";
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter, params ]( QRect const& rect )
        {
            QString s = "управления( от ";
            s += QString::number( params->MinControlPressure() ) + " до " + QString::number( params->MaxControlPressure() ) + " Бар )";
            painter.drawText( rect, s );
        });
    }
    return res;
}

}//namespace hydro

}//namespace test
