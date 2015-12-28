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
    OilTemp = mTemperature.T_oil;


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
        if (ReelControl())
            m25Result.Read();
        else
            m15Result.Read();
        auto f_d2d1 = []( double exp )
        {
            if ( exp < 30 )
                return 25.0*25.0 - 18.0*18.0;
            return 63.0*63.0-36.0*36.0;
        };

        double d2d1 = f_d2d1( test::servo::Parameters::Instance().DefaultExpenditure() );

        if ( ReelControl() )
        {
            for ( int i = 0; i < m1525Counts.OP15_25_count && i < m25Result.COORDINATE_COUNT; ++i )
            {
//                double xt = 0;
//                if ( i == 0 )
//                    xt = 0;
//                else if  ( i < 5 )
//                {
//                    int count = i + 1;
//                    double xtsr = 0;
//                    for ( int j = 0; j < count; ++j )
//                    {
//                        if ( j != 0 )
//                            xtsr += m25Result.coordinate[j] - m25Result.coordinate[ j-1 ];
//                    }
//                    xt = xtsr / count;
//                }
//                else
//                {
//                    int count = 5;
//                    double xtsr = 0;
//                    int start = i - ( count - 1 );
//                    int stop = i;
//                    for ( int j = start; j <= stop; ++j )
//                    {
//                        xtsr += m25Result.coordinate[j] - m25Result.coordinate[ j-1 ];
//                    }
//                    xt = xtsr / count;
//                }
                Data d;
                d.time = i;
//#warning уточнить формулу
//                d.position = 0.785 * xt * d2d1 / 1000000 * 60000;

                d.position = m25Result.coordinate[i];
                Graph->push_back( d );
            }
        }
        else
        {
            for ( int i = 0; i < m1525Counts.OP15_25_count && i < m15Result.COORDINATE_COUNT; ++i )
            {
                Data d;
                d.time = i;
                d.position = m15Result.coordinate[i];
                Graph->push_back( d );
            }
        }

        cpu::CpuMemory::Instance().DB31.SendContinue();
    }
}
bool TransientPerformance::Success() const
{
    return true;
}
QJsonObject TransientPerformance::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    QJsonArray a;
    foreach (Data const& d, Graph1)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("Graph1", a );

    QJsonArray b;
    foreach (Data const& d, Graph2)
    {
        b.insert( b.end(), d.Serialise() );
    }
    obj.insert("Graph2", b );

    QJsonArray c;
    foreach (Data const& d, Graph3)
    {
        c.insert( c.end(), d.Serialise() );
    }
    obj.insert("Graph3", c );

    return obj;
}
bool TransientPerformance::Deserialize( QJsonObject const& obj )
{
    Graph1.clear();
    Graph2.clear();
    Graph3.clear();
    QJsonArray a = obj.value("Graph1").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            Graph1.insert( Graph1.end(), d );
    }
    QJsonArray b = obj.value("Graph2").toArray();
    foreach (QJsonValue const& v, b)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            Graph2.insert( Graph2.end(), d );
    }
    QJsonArray c = obj.value("Graph3").toArray();
    foreach (QJsonValue const& v, c)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            Graph3.insert( Graph3.end(), d );
    }
    Test::Deserialize( obj );
    return true;
}

bool TransientPerformance::Draw( QPainter& painter, QRect &free_rect ) const
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
    [ this, &painter, &text_font, &DrawRowCenter, &metrix ]( QRect const& rect )
    {
        painter.save();

        ff0x::GraphBuilder::LinePoints data1;
        ff0x::GraphBuilder::LinePoints data2;
        ff0x::GraphBuilder::LinePoints data3;
        ff0x::GraphBuilder::LinePoints data1_e;
        ff0x::GraphBuilder::LinePoints data2_e;
        ff0x::GraphBuilder::LinePoints data3_e;

        double max_signal_1 = 0;
        double max_Leak_1 = 0;
        double min_signal_1 = 0;
        double min_Leak_1 = 0;

        double max_signal_2 = 0;
        double max_Leak_2 = 0;
        double min_signal_2 = 0;
        double min_Leak_2 = 0;


        double max_signal_3 = 0;
        double max_Leak_3 = 0;
        double min_signal_3 = 0;
        double min_Leak_3 = 0;

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("Graph1").toArray();
                foreach ( QJsonValue const& v, a )
                {
                    QJsonObject o = v.toObject();
                    data1_e.push_back( QPointF( o.value("time").toDouble(), o.value("expenditure").toDouble() ) );
                }
                QJsonArray b = obj.value("data").toObject().value("Graph2").toArray();
                foreach ( QJsonValue const& v, b )
                {
                    QJsonObject o = v.toObject();
                    data2_e.push_back( QPointF( o.value("time").toDouble(), o.value("expenditure").toDouble() ) );
                }
                QJsonArray c = obj.value("data").toObject().value("Graph3").toArray();
                foreach ( QJsonValue const& v, c )
                {
                    QJsonObject o = v.toObject();
                    data3_e.push_back( QPointF( o.value("time").toDouble(), o.value("expenditure").toDouble() ) );
                }

            }
        }

        for ( int i = 0; i < Graph1.size(); ++i )
        {
            Data const& item = Graph1[i];
            double abs_sig_a = std::abs( item.time );
            double abs_leak_a = std::abs( item.position );
            if ( i == 0 )
            {
                max_signal_1 = abs_sig_a;
                max_Leak_1 = abs_leak_a;
                min_signal_1 = abs_sig_a;
                min_Leak_1 = abs_leak_a;
            }
            else
            {
                if ( max_signal_1 < abs_sig_a )
                    max_signal_1 = abs_sig_a;

                if ( max_Leak_1 < abs_leak_a )
                    max_Leak_1 = abs_leak_a;

                if ( min_signal_1 > abs_sig_a )
                    min_signal_1 = abs_sig_a;

                if ( min_Leak_1 > abs_leak_a )
                    min_Leak_1 = abs_leak_a;
            }
            data1.push_back( QPointF( item.time, item.position ) );
        }
        for ( int i = 0; i < Graph2.size(); ++i )
        {
            Data const& item = Graph2[i];
            double abs_sig_a = std::abs( item.time );
            double abs_leak_a = std::abs( item.position );
            if ( i == 0 )
            {
                max_signal_2 = abs_sig_a;
                max_Leak_2 = abs_leak_a;
                min_signal_2 = abs_sig_a;
                min_Leak_2 = abs_leak_a;
            }
            else
            {
                if ( max_signal_2 < abs_sig_a )
                    max_signal_2 = abs_sig_a;

                if ( max_Leak_2 < abs_leak_a )
                    max_Leak_2 = abs_leak_a;

                if ( min_signal_2 > abs_sig_a )
                    min_signal_2 = abs_sig_a;

                if ( min_Leak_2 > abs_leak_a )
                    min_Leak_2 = abs_leak_a;
            }
            data2.push_back( QPointF( item.time, item.position ) );
        }
        for ( int i = 0; i < Graph3.size(); ++i )
        {
            Data const& item = Graph3[i];
            double abs_sig_a = std::abs( item.time );
            double abs_leak_a = std::abs( item.position );
            if ( i == 0 )
            {
                max_signal_3 = abs_sig_a;
                max_Leak_3 = abs_leak_a;
                min_signal_3 = abs_sig_a;
                min_Leak_3 = abs_leak_a;
            }
            else
            {
                if ( max_signal_3 < abs_sig_a )
                    max_signal_3 = abs_sig_a;

                if ( max_Leak_3 < abs_leak_a )
                    max_Leak_3 = abs_leak_a;

                if ( min_signal_3 > abs_sig_a )
                    min_signal_3 = abs_sig_a;

                if ( min_Leak_3 > abs_leak_a )
                    min_Leak_3 = abs_leak_a;
            }
            data3.push_back( QPointF( item.time, item.position ) );
        }

        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(data1, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(data2, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::darkCyan ) ) );
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(data3, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::darkGreen ) ) );
        if ( !data1_e.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(data1_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !data2_e.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(data2_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::darkYellow ) ) );
        if ( !data3_e.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(data3_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::darkMagenta ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        DrawRowCenter( p1t, text_font, Qt::black, "Переходные характеристики" );

        double max_signal = std::max( std::max( max_signal_1, max_signal_2), max_signal_3 );
        double min_signal = std::min( std::min( min_signal_1, min_signal_2), min_signal_3 );

        double max_leak = std::max( std::max( max_Leak_1, max_Leak_2), max_Leak_3 );
        double min_leak = std::min( std::min( min_Leak_1, min_Leak_2), min_Leak_3 );

        QPointF x_range_a( max_signal, min_signal );
        QPointF y_range_a( max_leak, min_leak );
        painter.drawPixmap( p1, builder.Draw( lines, x_range_a, y_range_a, ceil( max_signal - min_signal )/10, ceil(max_leak - min_leak)/10, "Время (мс)", "Расход (л/мин)", true ) );

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );

    free_rect.setHeight( 0 );
#warning TODO каждый график в своей сетке
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
