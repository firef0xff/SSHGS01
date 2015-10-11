#include "pressure_range.h"
#include <QJsonObject>
#include "../test_params_hydro.h"

namespace test
{

namespace hydro
{

PressureRange::PressureRange():
    test::hydro::Test( "Проверка диапазона давления управления\n(для направляющей гидроаппаратуры с электрогидравлическим управлением)", 7 ),
  Result(false)
{}

bool PressureRange::Run()
{
    return Result;
}

QJsonObject PressureRange::Serialise() const
{
    QJsonObject obj;
    obj.insert("Result", Result );

    return obj;
}
bool PressureRange::Deserialize( QJsonObject const& obj )
{
    Result = obj.value("Result").toBool();
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
        QString s = "Распределитель ";
        s += Result ? "" : "не";
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
    return res;
}

}//namespace hydro

}//namespace test
