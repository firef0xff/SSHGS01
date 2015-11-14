#include "va_characteristic.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
namespace test
{
namespace control_board
{

VACharacteristic::VACharacteristic():
    test::control_board::Test( "Построение зависимости выходного тока на катушку от входного опорного сигнала", 16 )
{}

bool VACharacteristic::Run()
{
    return true;
}
QJsonObject VACharacteristic::Serialise() const
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
bool VACharacteristic::Deserialize( QJsonObject const& obj )
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

bool VACharacteristic::Draw( QPainter& painter, QRect &free_rect ) const
{
    QFont header_font = painter.font();
    header_font.setFamily("Arial");
    header_font.setPointSize( 14 );
    QFont result_font = header_font;
    result_font.setUnderline(true);
    QFont text_font = header_font;
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
                                                    QString const& value = "")
    {
        painter.save();
        QFontMetrics metrix( font );
        QPoint start_point( place.left() , place.center().y()+metrix.height()/2 );
        QPoint start_point2( place.left() + metrix.width(label), place.center().y() +metrix.height()/2);
        painter.setFont( font );
        painter.setPen( color1 );
        painter.drawText( start_point, label );
        painter.setPen( color2 );
        painter.drawText( start_point2, value );
        painter.restore();
    };

    uint32_t num = 0;
    bool res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "2.Построение зависимости выходного тока на катушку" );
    }, 1.5 );
    res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "от входного опорного сигнала" );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});



    QFontMetrics metrix( text_font );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &DrawRowCenter, &metrix ]( QRect const& rect )
    {
        painter.save();

        ff0x::GraphBuilder::LinePoints data;
        ff0x::GraphBuilder::LinePoints data_e;

        double max_signal = 0;
        double max_Leak = 0;

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("Graph").toArray();
                foreach ( QJsonValue const& v, a )
                {
                    QJsonObject o = v.toObject();
                    data.push_back( QPointF( o.value("x").toDouble(), o.value("y").toDouble() ) );
                }
            }
        }

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
        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::GraphBuilder builder ( w, h, ff0x::GraphBuilder::PlusPlus, f );
        ff0x::GraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::GraphBuilder::Line(data, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data_e.empty() )
            lines.push_back( ff0x::GraphBuilder::Line(data_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        DrawRowCenter( p1t, text_font, Qt::black, "График" );
        painter.drawPixmap( p1, builder.Draw( lines, max_signal * 1.25, max_Leak * 1.25, 0.05, 0.5, "", "", true ) );

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );

    return res;
}


QJsonObject VACharacteristic::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("x", x );
    obj.insert("y", y );

    return obj;
}
bool VACharacteristic::Data::Deserialize( QJsonObject const& obj )
{
    x = obj.value("x").toDouble();
    y = obj.value("y").toDouble();
    return true;
}

}//namespace servo

}//namespace test
