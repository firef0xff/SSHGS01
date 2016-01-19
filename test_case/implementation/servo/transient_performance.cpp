#include "transient_performance.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
#include "../test_params_servo.h"
#include <thread>
namespace test
{
namespace servo
{

TransientPerformance::TransientPerformance():
    test::servo::Test( "Проверка переходных характеристик", 15, 25 )
{}

bool TransientPerformance::Run()
{
    Graph1.clear();
    Graph2.clear();
    Graph3.clear();
    Start();
    if ( ReelControl() )
        Wait( mControlReelBits.op25_ok, mControlReelBits.op25_end );
    else
        Wait( mControlBoardBits.op15_ok, mControlBoardBits.op15_end );
    if ( IsStopped() )
        return false;
    OilTemp = round( mTemperature.T_oil *100)/100;


    //dt = 1мс;
    //t = DB60, INT60 count - количестко показаний перемещений
    //диаметры цилиндров из параметров по заданному расходу

    return Success();
}
void TransientPerformance::UpdateData()
{
    Test::UpdateData();
    m1525Counts.Read();

    QVector<Data> *Graph = nullptr;
    if ( m1525Counts.OP15_25_Opor_1 && Graph1.empty() )
        Graph = &Graph1;
    if ( m1525Counts.OP15_25_Opor_2 && Graph2.empty() )
        Graph = &Graph2;
    if ( m1525Counts.OP15_25_Opor_3 && Graph3.empty() )
        Graph = &Graph3;

    if ( Graph )
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        m1525Counts.Read();
        m15Result.Read();

        for ( int i = 0; i < m1525Counts.OP15_25_count && i < m15Result.COORDINATE_COUNT; ++i )
        {
            Data d;
            d.time = i;
            d.position = m15Result.coordinate[i];
            Graph->push_back( d );
        }

        cpu::CpuMemory::Instance().DB31.SendContinue();
    }
}
bool TransientPerformance::Success() const
{
    return true;
}

namespace
{
QJsonArray ToJson( TransientPerformance::Source const& in_src )
{
    QJsonArray source;
    foreach ( TransientPerformance::Data const& d, in_src )
    {
        source.insert( source.end(), d.Serialise() );
    }

    return source;
}
TransientPerformance::Source FromJson( QJsonArray arr )
{
    TransientPerformance::Source res;
    foreach (QJsonValue const& v, arr)
    {
        TransientPerformance::Data d;
        if ( d.Deserialize( v.toObject() ) )
            res.insert( res.end(), d );
    }
    return std::move( res );
}

ff0x::GraphBuilder::LinePoints Process( TransientPerformance::Source const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::GraphBuilder::LinePoints result;

    auto f_d2d1 = []( double exp )
    {
        if ( exp < 30 )
            return 25.0*25.0 - 18.0*18.0;
        return 63.0*63.0-36.0*36.0;
    };

    double d2d1 = f_d2d1( test::servo::Parameters::Instance().DefaultExpenditure() );


    int aproximation_count = src.size() / 50 + 1;
    for ( int i = 0; i < src.size(); ++i )
    {
        double xt = 0;
        if ( i == 0 )
            xt = 0;
        else if  ( i < aproximation_count )
        {
            int count = i + 1;
            double xtsr = 0;
            for ( int j = 0; j < count; ++j )
            {
                if ( j != 0 )
                    xtsr += src[ j ].position - src[ j - 1 ].position;
            }
            xt = xtsr / count;
        }
        else
        {
            int count = aproximation_count;
            double xtsr = 0;
            int start = i - ( count - 1 );
            int stop = i;
            for ( int j = start; j <= stop; ++j )
            {
                xtsr += src[ j ].position - src[ j - 1 ].position;
            }
            xt = xtsr / count;
        }

        double x = src[i].time;
        double y = 0.785 * xt * d2d1 / 1000000 * 60000;

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

        result.push_back( QPointF( src[i].time, 0.785 * xt * d2d1 / 1000000 * 60000 ) );
    }
    return std::move( result );
}

}//namespace

QJsonObject TransientPerformance::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("Graph1", ToJson( Graph1 ) );
    obj.insert("Graph2", ToJson( Graph2 ) );
    obj.insert("Graph3", ToJson( Graph3 ) );
    return obj;
}
bool TransientPerformance::Deserialize( QJsonObject const& obj )
{
    Graph1 = FromJson( obj.value("Graph1").toArray() );
    Graph2 = FromJson( obj.value("Graph2").toArray() );
    Graph3 = FromJson( obj.value("Graph3").toArray() );
    Test::Deserialize( obj );
    return true;
}

bool TransientPerformance::Draw(QPainter& painter, QRect &free_rect , const QString &compare_width) const
{
    test::servo::Parameters *params = static_cast< test::servo::Parameters * >( CURRENT_PARAMS );
    if ( !params )
        return true;

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
        DrawRowCenter( rect, header_font, Qt::black, "6."+mName );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Данное испытание для аппаратов с пилотным гидравлическим управлением" );
    }, 1.5 );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, &params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Диапазон давлений при проведении испытаний, бар"), Qt::red, test::ToString( params->PressureNominal() ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, &params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Расход при проведении испытаний, л/мин"), Qt::red, test::ToString( params->DefaultExpenditure() ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Длительность испытания, сек"), Qt::red, test::ToString( TestingTime ) );
    }, 2 );


    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &result_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, result_font, Qt::black, "РЕЗУЛЬТАТ:" );
    }, 3 );


    QFontMetrics metrix( text_font );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width ]( QRect const& rect )
    {
        painter.save();

        ff0x::GraphBuilder::LinePoints data1;
        ff0x::GraphBuilder::LinePoints data1_e;
        ff0x::GraphBuilder::LinePoints data1_e2;
        ff0x::GraphBuilder::LinePoints data2;
        ff0x::GraphBuilder::LinePoints data2_e;
        ff0x::GraphBuilder::LinePoints data2_e2;


        QPointF x_range_1;
        QPointF y_range_1;

        QPointF x_range_1e;
        QPointF y_range_1e;

        QPointF x_range_1e2;
        QPointF y_range_1e2;

        QPointF x_range_2;
        QPointF y_range_2;

        QPointF x_range_2e;
        QPointF y_range_2e;

        QPointF x_range_2e2;
        QPointF y_range_2e2;


//        //поиск данных теста
//        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
//        {
//            auto obj = val.toObject();
//            if ( obj.value("id").toInt() == mId )
//            {
//                data1_e = Process( FromJson( obj.value("data").toObject().value("Graph1").toArray() ), x_range_1e, y_range_1e );
//                data2_e = Process( FromJson( obj.value("data").toObject().value("Graph1").toArray() ), x_range_2e, y_range_2e );
//            }
//        }
        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                data1_e2 = Process( FromJson( obj.value("data").toObject().value("Graph1").toArray() ), x_range_1e2, y_range_1e2 );
                data2_e2 = Process( FromJson( obj.value("data").toObject().value("Graph1").toArray() ), x_range_2e2, y_range_2e2 );
            }
        }

        data1 = Process( Graph1, x_range_1, y_range_1 );
        data2 = Process( Graph2, x_range_2, y_range_2 );


        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines1;
        ff0x::NoAxisGraphBuilder::GraphDataLine lines2;
        lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1,
                          ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );

        lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2,
                          ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );

        if ( !data1_e.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !data2_e.empty() )
            lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        if ( !data1_e2.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );
        if ( !data2_e2.empty() )
            lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());

        DrawRowCenter( p1t, text_font, Qt::black, "Переходные характеристики. Амплитуда 1" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;
            ff0x::DataLength( x_range_1,
                              x_range_1e, !data1_e.empty(),
                              x_range_1e2, !data1_e2.empty(),
                              x_range, x_step );
            ff0x::DataLength( y_range_1,
                              y_range_1e, !data1_e.empty(),
                              y_range_1e2, !data1_e2.empty(),
                              y_range, y_step );
            painter.drawPixmap( p1, builder.Draw( lines1, x_range, y_range, x_step, y_step, "Время (мс)", "Расход (л/мин)", true ) );
        }
        DrawRowCenter( p2t, text_font, Qt::black, "Переходные характеристики. Амплитуда 2" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;
            ff0x::DataLength( x_range_2,
                              x_range_2e, !data2_e.empty(),
                              x_range_2e2, !data2_e2.empty(),
                              x_range, x_step );
            ff0x::DataLength( y_range_2,
                              y_range_2e, !data2_e.empty(),
                              y_range_2e2, !data2_e2.empty(),
                              y_range, y_step );
            painter.drawPixmap( p2, builder.Draw( lines2, x_range, y_range, x_step, y_step, "Время (мс)", "Расход (л/мин)", true ) );
        }

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width ]( QRect const& rect )
    {
        painter.save();

        ff0x::GraphBuilder::LinePoints data3;
        ff0x::GraphBuilder::LinePoints data3_e;
        ff0x::GraphBuilder::LinePoints data3_e2;


        QPointF x_range_3;
        QPointF y_range_3;

        QPointF x_range_3e;
        QPointF y_range_3e;

        QPointF x_range_3e2;
        QPointF y_range_3e2;

//        //поиск данных теста
//        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
//        {
//            auto obj = val.toObject();
//            if ( obj.value("id").toInt() == mId )
//            {
//                data3_e = Process( FromJson( obj.value("data").toObject().value("Graph3").toArray() ), x_range_3e, y_range_3e );
//            }
//        }
        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                data3_e2 = Process( FromJson( obj.value("data").toObject().value("Graph3").toArray() ), x_range_3e2, y_range_3e2 );
            }
        }

        data3 = Process( Graph3, x_range_3, y_range_3 );


        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines3;
        lines3.push_back( ff0x::NoAxisGraphBuilder::Line(data3,
                          ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );

        if ( !data3_e.empty() )
            lines3.push_back( ff0x::NoAxisGraphBuilder::Line(data3_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        if ( !data3_e2.empty() )
            lines3.push_back( ff0x::NoAxisGraphBuilder::Line(data3_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());


        DrawRowCenter( p1t, text_font, Qt::black, "Переходные характеристики. Амплитуда 3" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;
            ff0x::DataLength( x_range_3,
                              x_range_3e, !data3_e.empty(),
                              x_range_3e2, !data3_e2.empty(),
                              x_range, x_step );
            ff0x::DataLength( y_range_3,
                              y_range_3e, !data3_e.empty(),
                              y_range_3e2, !data3_e2.empty(),
                              y_range, y_step );

            painter.drawPixmap( p1, builder.Draw( lines3, x_range, y_range, x_step, y_step, "Время (мс)", "Расход (л/мин)", true ) );
        }

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );

    free_rect.setHeight( 0 );
    return res;
}

QJsonObject TransientPerformance::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("time", time );
    obj.insert("position", position );

    return obj;
}
bool TransientPerformance::Data::Deserialize( QJsonObject const& obj )
{
    time = obj.value("time").toDouble();
    position = obj.value("position").toDouble();
    return true;
}

}//namespace servo

}//namespace test
