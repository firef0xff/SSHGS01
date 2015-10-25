#include "functional_test.h"
#include <QJsonObject>
#include "../test_params_hydro.h"

namespace test
{

namespace hydro
{

FunctionalTest::FunctionalTest():
    test::hydro::Test( "Испытание функционирования", 1 )
{}

bool FunctionalTest::Run()
{
    Start();
    Wait( mResults.OP1_Work, mResults.OP1_End );

    ReelA.work_on_max_pressure = mResults.OP1_Max_D_YESa && !mResults.OP1_Max_D_NOa;
    ReelA.work_on_min_pressure = mResults.OP1_Min_D_YESa && !mResults.OP1_Min_D_NOa;
    ReelA.I = mResults.OP1_Current_a;
    ReelA.P = mResults.OP1_Power_a;
    ReelA.R = mResults.OP1_Resist_a;
    ReelA.U = mResults.OP1_Voltage_a;


    ReelB.work_on_max_pressure = mResults.OP1_Max_D_YESb && !mResults.OP1_Max_D_NOb;
    ReelB.work_on_min_pressure = mResults.OP1_Min_D_YESb && !mResults.OP1_Min_D_NOb;
    ReelB.I = mResults.OP1_Current_b;
    ReelB.P = mResults.OP1_Power_b;
    ReelB.R = mResults.OP1_Resist_b;
    ReelB.U = mResults.OP1_Voltage_b;

    return false;
}

QJsonObject FunctionalTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("ReelA", ReelA.Serialise() );
    obj.insert("ReelB", ReelB.Serialise() );

    return obj;
}
bool FunctionalTest::Deserialize( QJsonObject const& obj )
{
    bool res = ReelA.Deserialize( obj.value("ReelA").toObject() );
    res *= ReelB.Deserialize( obj.value("ReelB").toObject() );
    return res;
}

bool FunctionalTest::Draw( QPainter& painter, QRect &free_rect ) const
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
        painter.drawText( rect, "Катушка А:" );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Функционирование при минимальном давлении: ";
        s += ReelA.work_on_min_pressure ? "Да" : "Нет";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Функционирование при максимальном давлении: ";
        s += ReelA.work_on_max_pressure ? "Да" : "Нет";
        painter.drawText( rect, s );
    });

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Ток I, А: ";
        s += QString::number( ReelA.I );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Напряжение U, В: ";
        s += QString::number( ReelA.U );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Сопротивление R, Ом: ";
        s += QString::number( ReelA.R );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Мощьность P, Ват: ";
        s += QString::number( ReelA.P );
        painter.drawText( rect, s );
    });

    if ( params->ReelCount() == 2 )
    {
        res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            painter.drawText( rect, "Катушка Б:" );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Функционирование при минимальном давлении: ";
            s += ReelB.work_on_min_pressure ? "Да" : "Нет";
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Функционирование при максимальном давлении: ";
            s += ReelB.work_on_max_pressure ? "Да" : "Нет";
            painter.drawText( rect, s );
        });

        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Ток I, А: ";
            s += QString::number( ReelB.I );
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Напряжение U, В: ";
            s += QString::number( ReelB.U );
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Сопротивление R, Ом: ";
            s += QString::number( ReelB.R );
            painter.drawText( rect, s );
        });
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Мощьность P, Ват: ";
            s += QString::number( ReelB.P );
            painter.drawText( rect, s );
        });
    }

    return res;
}

QJsonObject FunctionalTest::ReelResult::Serialise() const
{
    QJsonObject obj;

    obj.insert( "work_on_min_pressure", work_on_min_pressure );
    obj.insert( "work_on_max_pressure", work_on_max_pressure );

    obj.insert( "I", I );
    obj.insert( "U", U );
    obj.insert( "R", R );
    obj.insert( "P", P );

    return obj;
}
bool FunctionalTest::ReelResult::Deserialize( QJsonObject const& obj )
{
    work_on_min_pressure = obj.value( "work_on_min_pressure" ).toBool();
    work_on_max_pressure = obj.value( "work_on_max_pressure" ).toBool();

    I = obj.value( "I" ).toDouble();
    U = obj.value( "U" ).toDouble();
    R = obj.value( "R" ).toDouble();
    P = obj.value( "P" ).toDouble();

    return true;
}

}//namespace hydro

}//namespace test

