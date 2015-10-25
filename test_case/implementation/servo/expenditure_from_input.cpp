#include "expenditure_from_input.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
namespace test
{
namespace servo
{

ExpeditureFromInput::ExpeditureFromInput():
    test::servo::Test( "Зависимость расхода «к потребителю» от входного тока без нагрузки", 11 ),
    Gain(0),
    Hysteresis(0),
    Simmetry(0),
    Polar(0),
    Overlap(0)
{}

bool ExpeditureFromInput::Run()
{
    return true;
}
QJsonObject ExpeditureFromInput::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, Graph)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("Graph", a );
    obj.insert("Gain", Gain);
    obj.insert("Hysteresis", Hysteresis);
    obj.insert("Simmetry", Simmetry);
    obj.insert("Polar", Polar);
    obj.insert("Overlap", Overlap);

    return obj;
}
bool ExpeditureFromInput::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("Graph").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            Graph.insert( Graph.end(), d );
    }

    Gain = obj.value("Gain").toDouble();
    Hysteresis = obj.value("Hysteresis").toDouble();
    Simmetry = obj.value("Simmetry").toDouble();
    Polar = obj.value("Polar").toDouble();
    Overlap = obj.value("Overlap").toDouble();
    return true;
}

bool ExpeditureFromInput::Draw( QPainter& painter, QRect &free_rect ) const
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
        QString s = "Коэффициент усиления по расходу: " + QString::number( Gain );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Гистерезис: " + QString::number( Hysteresis );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Симмметрия: " + QString::number( Simmetry );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Полярность: " + QString::number( Polar );
        painter.drawText( rect, s );
    });
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "Характер перекрытия: " + QString::number( Overlap );
        painter.drawText( rect, s );
    });


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter ]( QRect const& rect )
    {
        QString s = "График опорного сигнала и расхода в канале";
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
            double abs_sig = std::abs( item.Signal );
            double abs_leak = std::abs( item.Expenditure );

            if ( max_signal < abs_sig )
                max_signal = abs_sig;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            data.push_back( QPointF( item.Signal, item.Expenditure ) );
        }

        ff0x::GraphBuilder builder ( 1024, 768, text_font );
        ff0x::GraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::GraphBuilder::Line( data, Qt::blue ) );
        painter.drawPixmap( rect, builder.Draw( lines, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "x", "y", true ) );
    }, free_rect.width()/4*3  );

    free_rect.setHeight( 0 );
    return res;
}

QJsonObject ExpeditureFromInput::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("Signal", Signal );
    obj.insert("Expenditure", Expenditure );

    return obj;
}
bool ExpeditureFromInput::Data::Deserialize( QJsonObject const& obj )
{
    Signal = obj.value("Signal").toDouble();
    Expenditure = obj.value("Expenditure").toDouble();
    return true;
}

}//namespace servo

}//namespace test
