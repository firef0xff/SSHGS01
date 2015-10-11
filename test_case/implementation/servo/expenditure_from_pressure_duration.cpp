#include "expenditure_from_pressure_duration.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
namespace test
{
namespace servo
{

ExpeditureFromPressureDuration::ExpeditureFromPressureDuration():
    test::servo::Test( "Зависимость расхода «к потребителю» от перепада давлений нагрузки", 12 )
{}

bool ExpeditureFromPressureDuration::Run()
{
    return true;
}
QJsonObject ExpeditureFromPressureDuration::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, BP5_3)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("BP5_3", a );

    QJsonArray b;
    foreach (Data const& d, BP3_V)
    {
        b.insert( b.end(), d.Serialise() );
    }
    obj.insert("BP3_V", a );

    return obj;
}
bool ExpeditureFromPressureDuration::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("BP5_3").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            BP5_3.insert( BP5_3.end(), d );
    }

    QJsonArray b = obj.value("BP3_V").toArray();
    foreach (QJsonValue const& v, b)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            BP3_V.insert( BP3_V.end(), d );
    }


    return true;
}

bool ExpeditureFromPressureDuration::Draw( QPainter& painter, QRect &free_rect ) const
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
        QString s = "Зависимость значения перепада от расхода в канале";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font ]( QRect const& rect )
    {
        ff0x::GraphBuilder::GraphDataType data;

        double max_signal = 0;
        double max_Leak = 0;

        foreach ( Data const& item, BP5_3 )
        {
            double abs_sig = std::abs( item.Expenditure );
            double abs_leak = std::abs( item.Duration );

            if ( max_signal < abs_sig )
                max_signal = abs_sig;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            data.push_back( QPointF( item.Expenditure, item.Duration ) );
        }

        ff0x::GraphBuilder builder ( 1024, 768, text_font );
        painter.drawPixmap( rect, builder.Draw( data, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "x", "y", true ) );
    }, free_rect.width()/4*3  );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Зависимость значения перепада от расхода в канале";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font ]( QRect const& rect )
    {
        ff0x::GraphBuilder::GraphDataType data;

        double max_signal = 0;
        double max_Leak = 0;

        foreach ( Data const& item, BP3_V )
        {
            double abs_sig = std::abs( item.Expenditure );
            double abs_leak = std::abs( item.Duration );

            if ( max_signal < abs_sig )
                max_signal = abs_sig;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            data.push_back( QPointF( item.Expenditure, item.Duration ) );
        }

        ff0x::GraphBuilder builder ( 1024, 768, text_font );
        painter.drawPixmap( rect, builder.Draw( data, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "x", "y", true ) );
    }, free_rect.width()/4*3  );

    free_rect.setHeight( 0 );
    return res;
}

QJsonObject ExpeditureFromPressureDuration::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("Duration", Duration );
    obj.insert("Expenditure", Expenditure );

    return obj;
}
bool ExpeditureFromPressureDuration::Data::Deserialize( QJsonObject const& obj )
{
    Duration = obj.value("Duration").toDouble();
    Expenditure = obj.value("Expenditure").toDouble();
    return true;
}

}//namespace servo

}//namespace test
