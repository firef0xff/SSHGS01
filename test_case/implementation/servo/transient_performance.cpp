#include "transient_performance.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
#include "../test_params_servo.h"
namespace test
{
namespace servo
{

TransientPerformance::TransientPerformance():
    test::servo::Test( "Проверка переходных характеристик", 15, 25 )
{}

bool TransientPerformance::Run()
{
    Graph.clear();
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
    if (ReelControl())
        m25Result.Read();
    else
        m15Result.Read();
    m1525Counts.Read();

    if ( m1525Counts.OP15_25_Opor_1 ||
         m1525Counts.OP15_25_Opor_2 ||
         m1525Counts.OP15_25_Opor_3)
    {
        auto f_d2d1 = []( double exp )
        {
            if ( exp < 30 )
                return (25.0 - 18.0)*(25.0 - 18.0);
            return (63.0-36.0)*(63.0-36.0);
        };

        double d2d1 = f_d2d1( test::servo::Parameters::Instance().DefaultExpenditure() );

        if ( ReelControl() )
        {
            for ( size_t i = 0; i < m1525Counts.OP15_25_count && i < m25Result.COORDINATE_COUNT; ++i )
            {
                Data d;
                d.time = i;
                d.expenditure = 10000.0/1270.0 * m25Result.coordinate[i] * d2d1;
                Graph.push_back( d );
            }
        }
        else
        {
            for ( size_t i = 0; i < m1525Counts.OP15_25_count && i < m15Result.COORDINATE_COUNT; ++i )
            {
                Data d;
                d.time = i;
                d.expenditure = 10000.0/1270.0 * m15Result.coordinate[i] * d2d1;
                Graph.push_back( d );
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
                    data_e.push_back( QPointF( o.value("time").toDouble(), o.value("expenditure").toDouble() ) );
                }
            }
        }

        foreach ( Data const& item, Graph )
        {
            double abs_sig = std::abs( item.time );
            double abs_leak = std::abs( item.expenditure );

            if ( max_signal < abs_sig )
                max_signal = abs_sig;

            if ( max_Leak < abs_leak )
                max_Leak = abs_leak;

            data.push_back( QPointF( item.time, item.expenditure ) );
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
        DrawRowCenter( p1t, text_font, Qt::black, "Переходные характеристики" );
        painter.drawPixmap( p1, builder.Draw( lines, max_signal * 1.25, max_Leak * 1.25, ceil(max_signal)/10, ceil(max_Leak)/10, "Время (мс)", "Расход (л/мин)", true ) );

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );

    free_rect.setHeight( 0 );
    return res;
}

QJsonObject TransientPerformance::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("time", time );
    obj.insert("expenditure", expenditure );

    return obj;
}
bool TransientPerformance::Data::Deserialize( QJsonObject const& obj )
{
    time = obj.value("time").toDouble();
    expenditure = obj.value("expenditure").toDouble();
    return true;
}

}//namespace servo

}//namespace test
