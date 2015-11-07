#include "activation_time.h"
#include <QJsonObject>
#include "../test_params_hydro.h"
#include "../../../stand_params.h"
#include <mutex>
namespace test
{

namespace hydro
{

ActivationTime::ActivationTime():
    test::hydro::Test( "Проверка времени срабатывания", 8 )
{}

bool ActivationTime::Run()
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;
    std::mutex mutex;
    std::unique_lock<std::mutex> lock( mutex );
    Launcher( std::bind( &ActivationTime::SetParams, this ) );
    mWaiter.wait( lock );
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

    return Success();
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
    QFont header_font = painter.font();
    header_font.setFamily("Arial");
    QFont result_font = header_font;
    result_font.setUnderline(true);
    QFont text_font = header_font;
    header_font.setPointSize( 14 );
    text_font.setPointSize( 12 );


    auto DrawRowCenter = [ &painter, &free_rect ](QRect const& place, QFont const& font, QColor const& color, QString const& text )
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.center().x() - metrix.width( text ) / 2, place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color );
        painter.drawText( start_point, text );
        painter.restore();
    };
    auto DrawRowLeft = [ &painter, &free_rect ](    QRect const& place,
                                                    QFont const& font,
                                                    QColor const& color1,
                                                    QString const& label,
                                                    QColor const& color2 = Qt::black,
                                                    QString const& value = "",
                                                    QColor const& color3 = Qt::black,
                                                    QString const& value2 = "")
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
        QPoint start_point2( start_point.x() + metrix.width(label), place.center().y() +metrix.height()/2);
        QPoint start_point3( start_point2.x() + metrix.width(value), place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color1 );
        painter.drawText( start_point, label );
        painter.setPen( color2 );
        painter.drawText( start_point2, value );
        painter.setPen( color3 );
        painter.drawText( start_point3, value2 );
        painter.restore();
    };

    QFontMetrics m(text_font);
    int width = m.width("123456789012345678901234567890123456789012345");
    char symbol = '.';
    auto FillToSize = [ width, &m, symbol ]( QString text )
    {
        while( m.width( text + symbol ) < width )
            text += symbol;
        return text + " ";
    };


    uint32_t num = 0;
    bool res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "8." +mName);
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Данное испытание проводится с целью определения времени срабатывания" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "включения и выключения. Время включения – время задержки между подачей" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "управляющего сигнала на катушку и перемещением золотника в требуемое крайнее" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "положение. Время выключение – время задержки между снятием с катушки сигнала" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "и перемещением золотника в нейтральное положение." );
    }, 1.5 );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, "что писать?" );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Расход при проведении испытаний, л/мин" ), Qt::black, "что писать?" );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Длительность испытания, сек"), Qt::red, "не реализовано" );
    }, 2 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &result_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, result_font, Qt::black, "РЕЗУЛЬТАТ:" );
    }, 3 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font, &FillToSize ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Время срабатывания катушки А, сек"), Qt::red, "значение? (норма 'значение?' л/мин)" );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font, &FillToSize ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Время срабатывания катушки А, сек"), Qt::red, "значение? (норма 'значение?' л/мин)" );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Испытание ", Qt::red, Success()? "пройдено" : "не пройдено" );
    }, 2 );

    return res;
}

bool ActivationTime::Success() const
{
    test::hydro::Parameters *params = static_cast< test::hydro::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return false;
    return ReelA.InTimeOn && ReelA.InTimeOff && ReelA.IsOn && ReelA.IsOff &&
            ( params->ReelCount() == 2 ? ReelB.InTimeOn && ReelB.InTimeOff && ReelB.IsOn && ReelB.IsOff : true );
}

void ActivationTime::SetParams()
{
    StandParams* ptr = new StandParams( false );
    mChildWindow.reset( ptr );
    ptr->SetCallback(
    [this]()
    {
        mWaiter.notify_one();
    });
    ptr->show();
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

