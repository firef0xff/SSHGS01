#include "power_test.h"
#include <QJsonObject>

namespace test
{

namespace control_board
{

PowerTest::PowerTest():
    test::control_board::Test( "Питание платы. Отсутствие ошибок", 15 ),
  Result(false)
{}

bool PowerTest::Run()
{
    return Result;
}

QJsonObject PowerTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("Result", Result );

    return obj;
}
bool PowerTest::Deserialize( QJsonObject const& obj )
{
    Result = obj.value("Result").toBool();
    return true;
}

bool PowerTest::Draw( QPainter& painter, QRect &free_rect ) const
{
    QFont header_font = painter.font();
    QFont text_font = painter.font();
    header_font.setPointSize( 14 );
    text_font.setPointSize( 12 );

    QFontMetrics head_metrix( header_font );

    uint32_t num = 0;
    bool res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &head_metrix, &header_font ]( QRect const& rect )
    {
        QPoint start_point( rect.center().x() - head_metrix.width( mName ) / 2, rect.bottom() );
        painter.setFont( header_font );
        painter.drawText( start_point, mName );
    });

    painter.setFont( text_font );
    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Питание платы ";
        s += Result ? "" : "не ";
        s += "корректно";
        painter.drawText( rect, s );
    });

    return res;
}

}//namespace hydro

}//namespace test
