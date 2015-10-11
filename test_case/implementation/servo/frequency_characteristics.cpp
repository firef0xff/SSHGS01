#include "frequency_characteristics.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
namespace test
{
namespace servo
{

FrequencyCharacteristics::FrequencyCharacteristics():
    test::servo::Test( "Частотные характеристики", 13 )
{}

bool FrequencyCharacteristics::Run()
{
    return true;
}
QJsonObject FrequencyCharacteristics::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, AFC)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("AFC", a );

    QJsonArray b;
    foreach (Data const& d, FFC)
    {
        b.insert( b.end(), d.Serialise() );
    }
    obj.insert("FFC", a );

    return obj;
}
bool FrequencyCharacteristics::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("AFC").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            AFC.insert( AFC.end(), d );
    }

    QJsonArray b = obj.value("FFC").toArray();
    foreach (QJsonValue const& v, b)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            FFC.insert( FFC.end(), d );
    }

    return true;
}

bool FrequencyCharacteristics::Draw( QPainter& painter, QRect &free_rect ) const
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
        QString s = "АЧХ";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font ]( QRect const& rect )
    {
        ff0x::GraphBuilder::GraphDataType data;

        double max_signal = 0;
        double max_Leak = 0;

        foreach ( Data const& item, AFC )
        {
            double abs_sig = std::abs( item.x );
            double abs_leak = std::abs( item.y );

            if ( max_signal < abs_sig )
                max_signal = abs_sig;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            data.push_back( QPointF( item.x, item.y ) );
        }

        ff0x::GraphBuilder builder ( 1024, 768, text_font );
        painter.drawPixmap( rect, builder.Draw( data, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "x", "y", true ) );
    }, free_rect.width()/4*3  );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "ФЧХ";
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font ]( QRect const& rect )
    {
        ff0x::GraphBuilder::GraphDataType data;

        double max_signal = 0;
        double max_Leak = 0;

        foreach ( Data const& item, FFC )
        {
            double abs_sig = std::abs( item.x );
            double abs_leak = std::abs( item.y );

            if ( max_signal < abs_sig )
                max_signal = abs_sig;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            data.push_back( QPointF( item.x, item.y ) );
        }

        ff0x::GraphBuilder builder ( 1024, 768, text_font );
        painter.drawPixmap( rect, builder.Draw( data, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "x", "y", true ) );
    }, free_rect.width()/4*3  );

    free_rect.setHeight( 0 );
    return res;
}


QJsonObject FrequencyCharacteristics::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("x", x );
    obj.insert("y", y );

    return obj;
}
bool FrequencyCharacteristics::Data::Deserialize( QJsonObject const& obj )
{
    y = obj.value("y").toDouble();
    x = obj.value("x").toDouble();
    return true;
}

}//namespace servo

}//namespace test
