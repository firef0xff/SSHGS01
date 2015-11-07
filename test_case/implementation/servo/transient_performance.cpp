#include "transient_performance.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
namespace test
{
namespace servo
{

TransientPerformance::TransientPerformance():
    test::servo::Test( "Переходные характеристики", 14 )
{}

bool TransientPerformance::Run()
{
    Data d;
    d.x = 1;
    d.y = 1.25;

    Graph.push_back( d );
    d.x += 0.5;
    d.y += 0.7;

    Graph.push_back( d );
    d.x += 0.5;
    d.y += 0.7;

    Graph.push_back( d );
    d.x += 0.5;
    d.y += 0.7;

    Graph.push_back( d );
    d.x += 0.5;
    d.y += 0.7;

    Graph.push_back( d );
    d.x += 0.5;
    d.y += 0.7;

    Graph.push_back( d );
    d.x += 0.5;
    d.y += 0.7;

    Graph.push_back( d );
    d.x += 0.5;
    d.y += 0.7;

    Graph.push_back( d );
    d.x += 0.5;
    d.y += 0.7;

    Graph.push_back( d );
    d.x += 0.5;
    d.y += 0.7;

    return true;
}
QJsonObject TransientPerformance::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, Graph)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("Graph", a );

    return obj;
}
bool TransientPerformance::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("Graph").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            Graph.insert( Graph.end(), d );
    }

    return true;
}

bool TransientPerformance::Draw( QPainter& painter, QRect &free_rect ) const
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
        QString s = "Переходные характеристики";
        painter.drawText( rect, s );
    });

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font ]( QRect const& rect )
    {
        ff0x::GraphBuilder::LinePoints data;

        double max_signal = 0;
        double max_Leak = 0;

        foreach ( Data const& item, Graph )
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
        ff0x::GraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::GraphBuilder::Line(data, Qt::blue) );
        painter.drawPixmap( rect, builder.Draw( lines, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "x", "y", true ) );
    }, 1, free_rect.width()/4*3  );

    free_rect.setHeight( 0 );

    return res;
}

QJsonObject TransientPerformance::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("x", x );
    obj.insert("y", y );

    return obj;
}
bool TransientPerformance::Data::Deserialize( QJsonObject const& obj )
{
    x = obj.value("x").toDouble();
    y = obj.value("y").toDouble();
    return true;
}

}//namespace servo

}//namespace test
