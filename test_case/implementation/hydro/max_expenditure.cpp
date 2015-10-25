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
    Wait( mResults.OP5_Work, mResults.OP5_End );

    ResultA = mResults.OP5_A_OK && !mResults.OP5_A_NO;
    ResultB = mResults.OP5_B_OK && !mResults.OP5_B_NO;

    return ResultA && (params->ReelCount() == 2 ? ResultB : true);
}

QJsonObject MaxExpenditureTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("ResultA", ResultA );
    obj.insert("ResultB", ResultB );

    return obj;
}
bool MaxExpenditureTest::Deserialize( QJsonObject const& obj )
{
    ResultA = obj.value("ResultA").toBool();
    ResultB = obj.value("ResultB").toBool();
    return true;
}

bool MaxExpenditureTest::Draw( QPainter& painter, QRect &free_rect ) const
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
        QString s = "Максимальный расход на канале А ";
        s += ResultA ? "" : "не ";
        s += "удовлетворяет установленным критериям";
        painter.drawText( rect, s );
    });

    if ( params->ReelCount() == 2 )
    {
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Максимальный расход на канале Б ";
            s += ResultB ? "" : "не ";
            s += "удовлетворяет установленным критериям";
            painter.drawText( rect, s );
        });
    }

    return res;
}
}//namespace hydro

}//namespace test
