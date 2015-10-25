#include "switchtest.h"
#include <QJsonObject>
#include "../test_params_hydro.h"

namespace test
{

namespace hydro
{

SwitchTest::SwitchTest():
    test::hydro::Test( "Проверка переключения запорно-регулирующего элемента пониженным напряжением", 6 ),
    ResultMinMinA(false),
    ResultMinMaxA(false),
    ResultMinMinB(false),
    ResultMinMaxB(false),
    ResultMaxMinA(false),
    ResultMaxMaxA(false),
    ResultMaxMinB(false),
    ResultMaxMaxB(false)
{}

bool SwitchTest::Run()
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;

    Start();
    Wait( mResults.OP6_Work, mResults.OP6_End );

    ResultMinMinA = mResults.OP6_MinD_MinUprD_YESa && !mResults.OP6_MinD_MinUprD_NOa;
    ResultMinMaxA = mResults.OP6_MinD_MaxUprD_YESa && !mResults.OP6_MinD_MaxUprD_NOa;

    ResultMinMinB = mResults.OP6_MinD_MinUprD_YESb && !mResults.OP6_MinD_MinUprD_NOb;
    ResultMinMaxB = mResults.OP6_MinD_MaxUprD_YESb && !mResults.OP6_MinD_MaxUprD_NOb;

    ResultMaxMinA = mResults.OP6_MaxD_MinUprD_YESa && !mResults.OP6_MaxD_MinUprD_NOa;
    ResultMaxMaxA = mResults.OP6_MaxD_MaxUprD_YESa && !mResults.OP6_MaxD_MaxUprD_NOa;

    ResultMaxMinB = mResults.OP6_MaxD_MinUprD_YESb && !mResults.OP6_MaxD_MinUprD_NOb;
    ResultMaxMaxB = mResults.OP6_MaxD_MaxUprD_YESb && !mResults.OP6_MaxD_MaxUprD_NOb;

    return ResultMinMinA && ResultMinMaxA && ResultMaxMinA && ResultMaxMaxA &&
            ( params->ReelCount() == 2 ? ResultMinMinB && ResultMinMaxB && ResultMaxMinB && ResultMaxMaxB : true );
}

QJsonObject SwitchTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("ResultMinMinA", ResultMinMinA );
    obj.insert("ResultMinMaxA", ResultMinMaxA );
    obj.insert("ResultMinMinB", ResultMinMinB );
    obj.insert("ResultMinMaxB", ResultMinMaxB );

    obj.insert("ResultMaxMinA", ResultMaxMinA );
    obj.insert("ResultMaxMaxA", ResultMaxMaxA );
    obj.insert("ResultMaxMinB", ResultMaxMinB );
    obj.insert("ResultMaxMaxB", ResultMaxMaxB );

    return obj;
}
bool SwitchTest::Deserialize( QJsonObject const& obj )
{
    ResultMinMinA = obj.value("ResultMinMinA").toBool();
    ResultMinMaxA = obj.value("ResultMinMaxA").toBool();
    ResultMinMinB = obj.value("ResultMinMinB").toBool();
    ResultMinMaxB = obj.value("ResultMinMaxB").toBool();

    ResultMaxMinA = obj.value("ResultMaxMinA").toBool();
    ResultMaxMaxA = obj.value("ResultMaxMaxA").toBool();
    ResultMaxMinB = obj.value("ResultMaxMinB").toBool();
    ResultMaxMaxB = obj.value("ResultMaxMaxB").toBool();
    return true;
}

bool SwitchTest::Draw( QPainter& painter, QRect &free_rect ) const
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
        QString s = "Позиция А: ";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Распределитель ";
        s += ResultMinMinA ? "" : "не ";
        s += "переключается при допустимом пониженном напряжении и пониженном давлении управления";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Распределитель ";
        s += ResultMinMaxA ? "" : "не ";
        s += "переключается при допустимом пониженном напряжении и повышенном давлении управления";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Распределитель ";
        s += ResultMaxMinA ? "" : "не ";
        s += "переключается при допустимом повышенном напряжении и пониженном давлении управления";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Распределитель ";
        s += ResultMaxMaxA ? "" : "не ";
        s += "переключается при допустимом повышенном напряжении и повышенном давлении управления";
        painter.drawText( rect, s );
    });

    if ( params->ReelCount() == 2 )
    {
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Позиция Б: ";
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Распределитель ";
            s += ResultMinMinB ? "" : "не ";
            s += "переключается при допустимом пониженном напряжении и пониженном давлении управления";
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Распределитель ";
            s += ResultMinMaxB ? "" : "не ";
            s += "переключается при допустимом пониженном напряжении и повышенном давлении управления";
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Распределитель ";
            s += ResultMaxMinB ? "" : "не ";
            s += "переключается при допустимом повышенном напряжении и пониженном давлении управления";
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Распределитель ";
            s += ResultMaxMaxB ? "" : "не ";
            s += "переключается при допустимом повышенном напряжении и повышенном давлении управления";
            painter.drawText( rect, s );
        });
    }

    return res;
}
}//namespace hydro

}//namespace test
