#include "max_expenditure.h"
#include <QJsonObject>

namespace test
{

namespace hydro
{

MaxExpenditureTest::MaxExpenditureTest():
    test::hydro::Test( "Проверка максимального расхода", 5 ),
  Result(false)
{}

bool MaxExpenditureTest::Run()
{
    return Result;
}

QJsonObject MaxExpenditureTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("Result", Result );

    return obj;
}
bool MaxExpenditureTest::Deserialize( QJsonObject const& obj )
{
    Result = obj.value("Result").toBool();
    return true;
}

bool MaxExpenditureTest::Draw( QPainter& painter, QRect &free_rect ) const
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
        QString s = "Максимальный расход ";
        s += Result ? "" : "не";
        s += "удовлетворяет установленным критериям";
        painter.drawText( rect, s );
    });

    return res;
}
}//namespace hydro

}//namespace test
