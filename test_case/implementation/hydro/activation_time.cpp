#include "activation_time.h"
#include <QJsonObject>
#include "../test_params_hydro.h"

namespace test
{

namespace hydro
{

ActivationTime::ActivationTime():
    test::hydro::Test( "Время срабатывания", 8 )
{}

bool ActivationTime::Run()
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;
#warning перед началом окно выбора динамики ?Oo
    params->WriteToController();

    Start();
    Wait( mResults.OP8_Work, mResults.OP8_End );

    ReelA.TimeOn = mResults.OP8_Time_on_a;
    ReelA.TimeOff = mResults.OP8_Time_off_a;
    ReelA.InTimeOn = mResults.OP8_open_YES_a && !mResults.OP8_open_NO_a;
    ReelA.InTimeOff = mResults.OP8_close_YES_a && !mResults.OP8_close_NO_a;
    ReelA.IsOn = !mResults.OP8_NO_Impuls_open_a;
    ReelA.IsOff = !mResults.OP8_NO_Impuls_close_a;

    ReelB.TimeOn = mResults.OP8_Time_on_b;
    ReelB.TimeOff = mResults.OP8_Time_off_b;
    ReelB.InTimeOn = mResults.OP8_open_YES_b && !mResults.OP8_open_NO_b;
    ReelB.InTimeOff = mResults.OP8_close_YES_b && !mResults.OP8_close_NO_b;
    ReelB.IsOn = !mResults.OP8_NO_Impuls_open_b;
    ReelB.IsOff = !mResults.OP8_NO_Impuls_close_b;

    OilTemp = mResults.Temperatura_masla;

    return ReelA.InTimeOn && ReelA.InTimeOff && ReelA.IsOn && ReelA.IsOff &&
            ( params->ReelCount() == 2 ? ReelB.InTimeOn && ReelB.InTimeOff && ReelB.IsOn && ReelB.IsOff : true );
}

QJsonObject ActivationTime::Serialise() const
{
    QJsonObject obj;
    obj.insert("ReelA", ReelA.Serialise() );
    obj.insert("ReelB", ReelB.Serialise() );
    obj.insert("OilTemp", OilTemp );

    return obj;
}
bool ActivationTime::Deserialize( QJsonObject const& obj )
{
    ReelA.Deserialize(obj.value("ReelA").toObject());
    ReelB.Deserialize(obj.value("ReelB").toObject());
    OilTemp = obj.value("OilTemp").toDouble();
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
    res = DrawLine( num, free_rect, text_font, [ this, &painter ]( QRect const& rect )
    {
        QString s = "Средняя температура масла во время испытания: " + QString::number( OilTemp );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Катушка А: время срабатывания ";
        s += ReelA.InTimeOn && ReelA.InTimeOff && ReelA.IsOn && ReelA.IsOff ? "" : "не ";
        s += "удовлетворяет";
        painter.drawText( rect, s );
    });

    if ( params->ReelCount() == 2 )
    {
        res = DrawLine( num, free_rect, text_font,
        [ this, &painter ]( QRect const& rect )
        {
            QString s = "Катушка Б: время срабатывания ";
            s += ReelB.InTimeOn && ReelB.InTimeOff && ReelB.IsOn && ReelB.IsOff ? "" : "не ";
            s += "удовлетворяет";
            painter.drawText( rect, s );
        });
    }

    return res;
}

QJsonObject ActivationTime::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("TimeOn", TimeOn );
    obj.insert("TimeOff", TimeOff );
    obj.insert("InTimeOn", InTimeOn );
    obj.insert("InTimeOff", InTimeOff );
    obj.insert("IsOn", IsOn );
    obj.insert("IsOff", IsOff );
    return obj;
}
bool ActivationTime::Data::Deserialize( QJsonObject const& obj )
{
    TimeOn = obj.value("TimeOn").toDouble();
    TimeOff = obj.value("TimeOff").toDouble();
    InTimeOn = obj.value("InTimeOn").toBool();
    InTimeOff = obj.value("InTimeOff").toBool();
    IsOn = obj.value("IsOn").toBool();
    IsOff = obj.value("IsOff").toBool();
    return true;
}

}//namespace hydro

}//namespace test

