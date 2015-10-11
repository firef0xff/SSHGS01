#include "switchtest.h"
#include <QJsonObject>

namespace test
{

namespace hydro
{

SwitchTest::SwitchTest():
    test::hydro::Test( "Проверка переключения запорно-регулирующего элемента пониженным напряжением", 6 ),
  Result(false)
{}

bool SwitchTest::Run()
{
    return Result;
}

QJsonObject SwitchTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("Result", Result );

    return obj;
}
bool SwitchTest::Deserialize( QJsonObject const& obj )
{
    Result = obj.value("Result").toBool();
    return true;
}

bool SwitchTest::Draw( QPainter& painter, QRect &free_rect ) const
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
        QString n = "Проверка переключения запорно-регулирующего";
        QPoint start_point( rect.center().x() - head_metrix.width( n ) / 2, rect.bottom() );
        painter.setFont( header_font );
        painter.drawText( start_point, n );
    });
    res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &head_metrix, &header_font ]( QRect const& rect )
    {
        QString n = "элемента пониженным напряжением";
        QPoint start_point( rect.center().x() - head_metrix.width( n ) / 2, rect.bottom() );
        painter.setFont( header_font );
        painter.drawText( start_point, n );
    });

    painter.setFont( text_font );
    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Распределитель ";
        s += Result ? "" : "не";
        s += "переключается при допустимом пониженном напряжении управления";
        painter.drawText( rect, s );
    });
    return res;
}
}//namespace hydro

}//namespace test
