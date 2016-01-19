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
    test::control_board::Test( "Построение зависимости выходного тока на катушку от входного опорного сигнала", 31 )
{}

bool VACharacteristic::Run()
{
    Start();
    Wait( mBits.op31_ok, mBits.op31_end );
    if ( IsStopped() )
        return false;

    for ( size_t i = 0; i < m31Results.SIGNAL_COUNT; ++i )
    {
        Data d;
        d.signal = m31Results.signal[i];
        d.current = m31Results.current[i];
        Graph.push_back(d);
    }

    OilTemp = round( mTemperature.T_oil *100)/100;
    return true;
}
void VACharacteristic::UpdateData()
{
    Test::UpdateData();
    m31Results.Read();
}
bool VACharacteristic::Success() const
{
    return true;
}

namespace
{

QJsonArray  ToJson ( VACharacteristic::DataSet const& data )
{
    QJsonArray arr;

    foreach ( VACharacteristic::Data const& d, data )
    {
        arr.insert( arr.end(), d.Serialise() );
    }

    return std::move( arr );
}
VACharacteristic::DataSet FromJson( QJsonArray const& arr )
{
    VACharacteristic::DataSet data;

    foreach (QJsonValue const& v, arr)
    {
        VACharacteristic::Data d;
        if ( d.Deserialize( v.toObject() ) )
            data.insert( data.end(), d );
    }

    return std::move( data );
}

ff0x::NoAxisGraphBuilder::LinePoints Process ( VACharacteristic::DataSet const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( int i = 0; i < src.size(); ++i )
    {
        double const& x = fabs( src[i].signal );
        double const& y = fabs( src[i].current );

        if ( !i )
        {
            x_range.setX( x );
            x_range.setY( x );
            y_range.setX( y );
            y_range.setY( y );
        }
        else
        {
            if ( x > x_range.x() )
                x_range.setX( x );
            if ( x < x_range.y() )
                x_range.setY( x );

            if ( y > y_range.x() )
                y_range.setX( y );
            if ( y < y_range.y() )
                y_range.setY( y );
        }

        result.push_back( QPointF( x, y ) );
    }
    return std::move( result );
}

}//namespace


QJsonObject VACharacteristic::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("Graph", ToJson( Graph ) );

    return obj;
}
bool VACharacteristic::Deserialize( QJsonObject const& obj )
{
    Graph = FromJson( obj.value("Graph").toArray() );
    Test::Deserialize( obj );
    return true;
}

bool VACharacteristic::Draw(QPainter& painter, QRect &free_rect , const QString &compare_width) const
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
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width ]( QRect const& rect )
    {
        painter.save();

        ff0x::NoAxisGraphBuilder::LinePoints data;
        ff0x::NoAxisGraphBuilder::LinePoints data_e;
        ff0x::NoAxisGraphBuilder::LinePoints data_e2;

        QPointF x_range_1;
        QPointF y_range_1;

        QPointF x_range_1e;
        QPointF y_range_1e;

        QPointF x_range_1e2;
        QPointF y_range_1e2;

//        //поиск данных теста
//        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
//        {
//            auto obj = val.toObject();
//            if ( obj.value("id").toInt() == mId )
//            {
//                QJsonArray a = obj.value("data").toObject().value("Graph").toArray();
//                data_e = Process( FromJson( a ), x_range_1e, y_range_1e );
//            }
//        }

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("Graph").toArray();
                data_e2 = Process( FromJson( a ), x_range_1e2, y_range_1e2 );
            }
        }


        data = Process( Graph, x_range_1, y_range_1 );

        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(data, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data_e.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(data_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !data_e2.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(data_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );


        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        DrawRowCenter( p1t, text_font, Qt::black, "График" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;

            ff0x::DataLength( x_range_1,
                              x_range_1e, !data_e.empty(),
                              x_range_1e2, !data_e2.empty(),
                              x_range, x_step );
            ff0x::DataLength( y_range_1,
                              y_range_1e, !data_e.empty(),
                              y_range_1e2, !data_e2.empty(),
                              y_range, y_step );

            painter.drawPixmap( p1, builder.Draw( lines, x_range, y_range, x_step, y_step, "Опорный сигнал", "Выходной ток", true ) );
        }

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );

    return res;
}


QJsonObject VACharacteristic::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("signal", signal );
    obj.insert("current", current );

    return obj;
}
bool VACharacteristic::Data::Deserialize( QJsonObject const& obj )
{
    signal = obj.value("signal").toDouble();
    current = obj.value("current").toDouble();
    return true;
}

}//namespace servo

}//namespace test
