#include "activation_time.h"
#include <QJsonObject>
#include "../test_params_hydro.h"

namespace test
{

namespace hydro
{

ActivationTime::ActivationTime():
    test::hydro::Test( "Время срабатывания", 8 ),
  ResultA(false),
  ResultB(false)
{}

bool ActivationTime::Run()
{
    return ResultA && ResultB;
}

QJsonObject ActivationTime::Serialise() const
{
    QJsonObject obj;
    obj.insert("ResultA", ResultA );
    obj.insert("ResultB", ResultB );

    return obj;
}
bool ActivationTime::Deserialize( QJsonObject const& obj )
{
    ResultA = obj.value("ResultA").toBool();
    ResultB = obj.value("ResultB").toBool();

    return true;
}

bool ActivationTime::Draw( QPainter& painter, QRect &free_rect ) const
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
        QPoint start_point( rect.center().x() - head_metrix.width( mName ) / 2, rect.bottom() );
        painter.setFont( header_font );
        painter.drawText( start_point, mName );
    });

    painter.setFont( text_font );
    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Катушка А: время срабатывания ";
        s += ResultA ? "" : "не";
        s += "удовлетворяет";
        painter.drawText( rect, s );
    });

    if ( params->ReelCount() == 2 )
    {
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Катушка Б: время срабатывания ";
            s += ResultB ? "" : "не";
            s += "удовлетворяет";
            painter.drawText( rect, s );
        });
    }

    return res;
}

}//namespace hydro

}//namespace test

