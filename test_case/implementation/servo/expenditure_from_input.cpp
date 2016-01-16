#include "expenditure_from_input.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
#include "test_case/implementation/test_params_servo.h"
#include "cmath"
#include <thread>

namespace test
{
namespace servo
{

ExpeditureFromInput::ExpeditureFromInput():
    test::servo::Test( "Зависимость расхода «к потребителю» от входного тока без нагрузки", 12, 22 )
{}

bool ExpeditureFromInput::Run()
{
    level = 0;
    GraphA1.clear();
    GraphB1.clear();
    GraphA2.clear();
    GraphB2.clear();
    Start();
    if ( ReelControl() )
        Wait( mControlReelBits.op22_ok, mControlReelBits.op22_end );
    else
        Wait( mControlBoardBits.op12_ok, mControlBoardBits.op12_end );
    if ( IsStopped() )
        return false;

    OilTemp = round( mTemperature.T_oil *100)/100;

    return Success();
}
void ExpeditureFromInput::UpdateData()
{
    servo::Parameters &params = test::servo::Parameters::Instance();
    Test::UpdateData();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    bool *ready = 0;
    if ( ReelControl() )
    {
        ready = &mControlReelBits.op24_ready;
    }
    else
    {
        ready = &mControlBoardBits.op14_ready;
    }

    auto ConvertData = [this]( DataSet *first, DataSet *second  )
    {
        for ( int i = 0; i < m12Results.OPEN_REF_COUNT; ++i )
        {
            Data d;
            d.Expenditure = m12Results.open_consumption[i];
            d.Signal = m12Results.open_ref[i];
            first->push_back( d );
            d.Expenditure = m12Results.close_consumption[i];
            d.Signal = m12Results.close_ref[i];
            second->push_front( d );
        }
    };

    if ( *ready )
    {
        m12Results.Read();
        m1525Counts.Read();
        switch ( level )
        {
        case 0:
            if ( m1525Counts.OP15_25_Opor_1 )
            {
                if ( params.TestChannelA() && params.SignalOnChannelA() == CS_REEL_A )
                {// P -> A
                    ConvertData( &GraphA1, &GraphA2 );
                }
                if ( params.TestChannelB() && params.SignalOnChannelA() == CS_REEL_B )
                {// P -> B
                    ConvertData( &GraphB1, &GraphB2 );
                }
                ++level;
                cpu::CpuMemory::Instance().DB31.SendContinue();
            }            
            break;
        case 1:
            if ( m1525Counts.OP15_25_Opor_2 )
            {
                if ( params.TestChannelA() && params.SignalOnChannelB() == CS_REEL_A )
                {// P -> A
                    ConvertData( &GraphA1, &GraphA2 );
                }
                if ( params.TestChannelB()  && params.SignalOnChannelB() == CS_REEL_B )
                {// P -> B
                    ConvertData( &GraphB1, &GraphB2 );
                }
                ++level;
                cpu::CpuMemory::Instance().DB31.SendContinue();
            }
            break;
        default:
            ++level;
            break;
        }
    }
}
bool ExpeditureFromInput::Success() const
{
    return true;
}

namespace
{

QJsonArray ToJson( ExpeditureFromInput::DataSet const& data )
{
    QJsonArray arr;
    foreach (ExpeditureFromInput::Data const& d, data)
    {
        arr.insert( arr.end(), d.Serialise() );
    }
    return std::move( arr );
}
ExpeditureFromInput::DataSet FromJson( QJsonArray const& arr )
{
    ExpeditureFromInput::DataSet data;
    foreach (QJsonValue const& v, arr)
    {
        ExpeditureFromInput::Data d;
        if ( d.Deserialize( v.toObject() ) )
            data.insert( data.end(), d );
    }
    return std::move( data );
}

double CalckGain( ExpeditureFromInput::DataSet const& data )
{
    if ( !data.size() )
        return 0;
    int pos_max_exp = 0;
    int pos_min_exp = 0;
    int pos_min_signal = 0;
    for ( int i = 0; i < data.size(); ++i )
    {
        if ( data[i].Expenditure > data[pos_max_exp].Expenditure )
            pos_max_exp = i;
        if ( data[i].Expenditure < data[pos_min_exp].Expenditure )
            pos_min_exp = i;
    }
    double range = data[pos_max_exp].Expenditure - data[pos_min_exp].Expenditure;
    for ( int i = 0; i < data.size(); ++i )
    {
        if ( i )
        {
            double dist = (data[i].Expenditure - data[ 0 ].Expenditure);

            if ( dist > range / 100 * 0.5 &&
                 i < data.size() / 2 &&
                 !pos_min_signal)
            {
                pos_min_signal = i;
            }
        }
    }
/// Коэффициент усиления по расходу
///     1. сигнал (x_p0) при котором расход равен 0 ( а, б )
///     2. сигнал (x_max) при котором достигнут максимальны расход (q_max) ( максимальный расход см из массива ) ( а, б )
///     3. коффициент = Q_max/( x_max - x_p0 )
    double Q = data[pos_max_exp].Expenditure;
    double x_max = data[pos_max_exp].Signal;
    double x_p0 = data[pos_min_signal].Signal;
    return Q/fabs( x_max - x_p0 );
}
double CalckNonlinearity( ExpeditureFromInput::DataSet const& data )
{
    if ( !data.size() )
        return 0;

    int pos_max_exp = 0;
    int pos_min_exp = 0;
    int pos_min_signal = 0;
    for ( int i = 0; i < data.size(); ++i )
    {
        if ( data[i].Expenditure > data[pos_max_exp].Expenditure )
            pos_max_exp = i;
        if ( data[i].Expenditure < data[pos_min_exp].Expenditure )
            pos_min_exp = i;
    }
    double range = data[pos_max_exp].Expenditure - data[pos_min_exp].Expenditure;
    for ( int i = 0; i < data.size(); ++i )
    {
        if ( i )
        {
            double dist = (data[i].Expenditure - data[ 0 ].Expenditure);

            if ( dist > range / 100 * 0.5 &&
                 i < data.size() / 2 &&
                 !pos_min_signal)
            {
                pos_min_signal = i;
            }
        }
    }
///     Нелинейность
///     1. q_max - расход при максимальном управляющем сигнале x_max (максимально значение управляющего сигнала)
///        q_0   - расход при при управляющем сигнале x_0 (соответствующем нулевому расположению распеделителя) ??????
///     2. вычислить коэффициент k = (q_max - q_0) / ( x_max - x_0 )
///     3. для каждого управляющего сигнала вычислить q_et[i] = k * x[i];
///     4. вычислить массив r[i] = q_et[i] - q[i];
///     5. результат равен max( r[i] ) / q_max * 100;

    double q_max = data[pos_max_exp].Expenditure;
    double q_0 = data[pos_min_signal].Expenditure;
    double x_max = data[pos_max_exp].Signal;
    double x_0 = data[pos_min_signal].Signal;

    double k = fabs( (q_max - q_0) / ( x_max - x_0 ) );

    double max_r = 0 ;
    for ( int i = pos_min_signal; i < pos_max_exp; ++i )
    {
        double const& x = data[i].Signal;
        double const& q = data[i].Expenditure;

        double r = k * fabs( x - data[ pos_min_signal ].Signal ) - q;
        if ( i == pos_min_signal )
            max_r = r;
        else if ( r > max_r )
            max_r = r;
    }
    return max_r / q_max * 100;
}
double CalckHysteresis( ExpeditureFromInput::DataSet const& data1, ExpeditureFromInput::DataSet const& data2 )
{
    if ( !data1.size() || !data2.size() )
        return 0;

    double delta = 0;
    int q_max_pos = 0;
    for ( int i = 0; i < data1.size(); ++i )
    {
        double q1 = data1[ i ].Expenditure;
        double q2 = data2[ i ].Expenditure;

        double d = fabs( q1 - q2 );

        if ( delta < d  )
            delta = d;
        if ( q1 > data1[q_max_pos].Expenditure )
            q_max_pos = i;
    }
///     Гистерезис
///     1. max( q1[x] - q2[x] ) / q_max * 100;
    return delta / data1[q_max_pos].Expenditure * 100.0;
}

ff0x::NoAxisGraphBuilder::LinePoints Process ( ExpeditureFromInput::DataSet const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( int i = 0; i < src.size(); ++i )
    {
        double const& x = src[i].Signal;
        double const& y = src[i].Expenditure;

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

ff0x::NoAxisGraphBuilder::LinePoints ProcessET ( ExpeditureFromInput::DataSet const& data, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    if ( !data.size() )
        return std::move( result );;

    int pos_max_exp = 0;
    int pos_min_exp = 0;
    int pos_min_signal = 0;
    for ( int i = 0; i < data.size(); ++i )
    {
        if ( data[i].Expenditure > data[pos_max_exp].Expenditure )
            pos_max_exp = i;
        if ( data[i].Expenditure < data[pos_min_exp].Expenditure )
            pos_min_exp = i;
    }
    double range = data[pos_max_exp].Expenditure - data[pos_min_exp].Expenditure;
    for ( int i = 0; i < data.size(); ++i )
    {
        if ( i )
        {
            double dist = (data[i].Expenditure - data[ 0 ].Expenditure);

            if ( dist > range / 100 * 0.5 &&
                 i < data.size() / 2 &&
                 !pos_min_signal)
            {
                pos_min_signal = i;
            }
        }
    }
///     Нелинейность
///     1. q_max - расход при максимальном управляющем сигнале x_max (максимально значение управляющего сигнала)
///        q_0   - расход при при управляющем сигнале x_0 (соответствующем нулевому расположению распеделителя) ??????
///     2. вычислить коэффициент k = (q_max - q_0) / ( x_max - x_0 )
///     3. для каждого управляющего сигнала вычислить q_et[i] = k * x[i];
///     4. вычислить массив r[i] = q_et[i] - q[i];
///     5. результат равен max( r[i] ) / q_max * 100;

    double q_max = data[pos_max_exp].Expenditure;
    double q_0 = data[pos_min_signal].Expenditure;
    double x_max = data[pos_max_exp].Signal;
    double x_0 = data[pos_min_signal].Signal;

    double k = fabs( (q_max - q_0) / ( x_max - x_0 ) );


    for ( int i = pos_min_signal; i < pos_max_exp; ++i )
    {
        double const& x = data[i].Signal;
        double y = k * fabs( x - data[ pos_min_signal ].Signal );

        if ( i == pos_min_signal )
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

QJsonObject ExpeditureFromInput::Serialise() const
{
    QJsonObject obj = Test::Serialise();

    obj.insert("GraphA1", ToJson( GraphA1 ) );
    obj.insert("GraphA2", ToJson( GraphA2 ) );

    obj.insert("GraphB1", ToJson( GraphB1 ) );
    obj.insert("GraphB2", ToJson( GraphB2 ) );

    return obj;
}
bool ExpeditureFromInput::Deserialize( QJsonObject const& obj )
{
    GraphA1 = FromJson( obj.value("GraphA1").toArray() );
    GraphA2 = FromJson( obj.value("GraphA2").toArray() );
    GraphB1 = FromJson( obj.value("GraphB1").toArray() );
    GraphB2 = FromJson( obj.value("GraphB2").toArray() );

    Test::Deserialize( obj );
    return true;
}

bool ExpeditureFromInput::Draw(QPainter& painter, QRect &free_rect , const QString &compare_width) const
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
        DrawRowCenter( rect, header_font, Qt::black, "3.Зависимость расхода «к потребителю»" );
    }, 1.5 );
    res = DrawLine( num, free_rect, header_font,
    [ this, &painter, &DrawRowCenter, &header_font ]( QRect const& rect )
    {
        DrawRowCenter( rect, header_font, Qt::black, "от входного тока без нагрузки" );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "При этом испытании получают кривую зависимости расхода без нагрузки от" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "входного управляющего сигнала, которая используется для получения многих" );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "статических показателей аппарата." );
    }, 1.5 );



    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, &params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, test::ToString( params->PressureNominal() ) );
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

        ff0x::NoAxisGraphBuilder::LinePoints dataA1;
        ff0x::NoAxisGraphBuilder::LinePoints dataA1_e;
        ff0x::NoAxisGraphBuilder::LinePoints dataA1_e2;
        ff0x::NoAxisGraphBuilder::LinePoints dataA2;
        ff0x::NoAxisGraphBuilder::LinePoints dataA2_e;
        ff0x::NoAxisGraphBuilder::LinePoints dataA2_e2;

        ff0x::NoAxisGraphBuilder::LinePoints dataB1;
        ff0x::NoAxisGraphBuilder::LinePoints dataB1_e;
        ff0x::NoAxisGraphBuilder::LinePoints dataB1_e2;
        ff0x::NoAxisGraphBuilder::LinePoints dataB2;
        ff0x::NoAxisGraphBuilder::LinePoints dataB2_e;
        ff0x::NoAxisGraphBuilder::LinePoints dataB2_e2;

        QPointF x_range_a1;
        QPointF y_range_a1;
        QPointF x_range_a1e;
        QPointF y_range_a1e;
        QPointF x_range_a1e2;
        QPointF y_range_a1e2;

        QPointF x_range_a2;
        QPointF y_range_a2;
        QPointF x_range_a2e;
        QPointF y_range_a2e;
        QPointF x_range_a2e2;
        QPointF y_range_a2e2;

        QPointF x_range_b1;
        QPointF y_range_b1;
        QPointF x_range_b1e;
        QPointF y_range_b1e;
        QPointF x_range_b1e2;
        QPointF y_range_b1e2;

        QPointF x_range_b2;
        QPointF y_range_b2;
        QPointF x_range_b2e;
        QPointF y_range_b2e;
        QPointF x_range_b2e2;
        QPointF y_range_b2e2;

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                dataA1_e = Process( FromJson( obj.value("data").toObject().value("GraphA1").toArray() ), x_range_a1e, y_range_a1e );
                dataA2_e = Process( FromJson( obj.value("data").toObject().value("GraphA2").toArray() ), x_range_a2e, y_range_a2e );
                dataB1_e = Process( FromJson( obj.value("data").toObject().value("GraphB1").toArray() ), x_range_b1e, y_range_b1e );
                dataB2_e = Process( FromJson( obj.value("data").toObject().value("GraphB2").toArray() ), x_range_b2e, y_range_b2e );
            }
        }
        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                dataA1_e2 = Process( FromJson( obj.value("data").toObject().value("GraphA1").toArray() ), x_range_a1e2, y_range_a1e2 );
                dataA2_e2 = Process( FromJson( obj.value("data").toObject().value("GraphA2").toArray() ), x_range_a2e2, y_range_a2e2 );
                dataB1_e2 = Process( FromJson( obj.value("data").toObject().value("GraphB1").toArray() ), x_range_b1e2, y_range_b1e2 );
                dataB2_e2 = Process( FromJson( obj.value("data").toObject().value("GraphB2").toArray() ), x_range_b2e2, y_range_b2e2 );
            }
        }

//        dataA1_e = ProcessET( GraphA1, x_range_a1e, y_range_a1e );
//        dataB1_e = ProcessET( GraphB1, x_range_b1e, y_range_b1e );
        dataA1 = Process( GraphA1, x_range_a1, y_range_a1 );
        dataA2 = Process( GraphA2, x_range_a2, y_range_a2 );
        dataB1 = Process( GraphB1, x_range_b1, y_range_b1 );
        dataB2 = Process( GraphB2, x_range_b2, y_range_b2 );

        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines_a;
        lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA1, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат. Прямой ход", Qt::blue ) ) );
        if ( !dataA1_e.empty() )
            lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA1_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон. Прямой ход", Qt::red ) ) );
        if ( !dataA1_e2.empty() )
            lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA1_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат. Прямой ход", Qt::gray ) ) );

        lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA2, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат. Обратный ход", Qt::darkGreen ) ) );
        if ( !dataA2_e.empty() )
            lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA2_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон. Обратный ход", Qt::darkRed ) ) );
        if ( !dataA2_e2.empty() )
            lines_a.push_back( ff0x::NoAxisGraphBuilder::Line(dataA2_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат. Обратный ход", Qt::darkGray ) ) );

        ff0x::NoAxisGraphBuilder::GraphDataLine lines_b;
        lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB1, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат. Прямой ход", Qt::blue ) ) );
        if ( !dataB1_e.empty() )
            lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB1_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон. Прямой ход", Qt::red ) ) );
        if ( !dataB1_e2.empty() )
            lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB1_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат. Прямой ход", Qt::gray ) ) );

        lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB2, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат. Обратный ход", Qt::darkGreen ) ) );
        if ( !dataB2_e.empty() )
            lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB2_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон. Обратный ход", Qt::darkRed ) ) );
        if ( !dataB2_e2.empty() )
            lines_b.push_back( ff0x::NoAxisGraphBuilder::Line(dataB2_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат. Обратный ход", Qt::darkGray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());
        DrawRowCenter( p1t, text_font, Qt::black, "P->A" );
        {
            QPointF x_test_range;
            QPointF y_test_range;

            x_test_range.setX( std::max( x_range_a1.x(), x_range_a2.x() ) );
            x_test_range.setY( std::min( x_range_a1.y(), x_range_a2.y() ) );

            y_test_range.setX( std::max( y_range_a1.x(), y_range_a2.x() ) );
            y_test_range.setY( std::min( y_range_a1.y(), y_range_a2.y() ) );

            if ( !dataA1_e.empty() )
            {
                x_test_range.setX( std::max( x_range_a1e.x(), x_test_range.x() ) );
                x_test_range.setY( std::min( x_range_a1e.y(), x_test_range.y() ) );

                y_test_range.setX( std::max( y_range_a1e.x(), y_test_range.x() ) );
                y_test_range.setY( std::min( y_range_a1e.y(), y_test_range.y() ) );
            }
            if ( !dataA2_e.empty() )
            {
                x_test_range.setX( std::max( x_range_a2e.x(), x_test_range.x() ) );
                x_test_range.setY( std::min( x_range_a2e.y(), x_test_range.y() ) );

                y_test_range.setX( std::max( y_range_a2e.x(), y_test_range.x() ) );
                y_test_range.setY( std::min( y_range_a2e.y(), y_test_range.y() ) );
            }

            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;

            ff0x::DataLength( x_range_a1,
                              x_range_a1e, !dataA1_e.empty(),
                              x_range_a1e2, !dataA1_e2.empty(),
                              x_range_a2, true,
                              x_range_a2e, !dataA2_e.empty(),
                              x_range_a2e2, !dataA2_e2.empty(),
                              x_range, x_step );
            ff0x::DataLength( y_range_a1,
                              y_range_a1e, !dataA1_e.empty(),
                              y_range_a1e2, !dataA1_e2.empty(),
                              y_range_a2, true,
                              y_range_a2e, !dataA2_e.empty(),
                              y_range_a2e2, !dataA2_e2.empty(),
                              y_range, y_step );

            painter.drawPixmap( p1, builder.Draw( lines_a, x_range, y_range, x_step, y_step, "Опорный сигнал", "Расход (л/мин)", true ) );
        }
        DrawRowCenter( p2t, text_font, Qt::black, "P->B" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;

            ff0x::DataLength( x_range_b1,
                              x_range_b1e, !dataA1_e.empty(),
                              x_range_b1e2, !dataA1_e2.empty(),
                              x_range_b2, true,
                              x_range_b2e, !dataA2_e.empty(),
                              x_range_b2e2, !dataA2_e2.empty(),
                              x_range, x_step );
            ff0x::DataLength( y_range_b1,
                              y_range_b1e, !dataA1_e.empty(),
                              y_range_b1e2, !dataA1_e2.empty(),
                              y_range_b2, true,
                              y_range_b2e, !dataA2_e.empty(),
                              y_range_b2e2, !dataA2_e2.empty(),
                              y_range, y_step );

            painter.drawPixmap( p2, builder.Draw( lines_b, x_range, y_range, x_step, y_step, "Опорный сигнал", "Расход (л/мин)", true ) );
        }

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Канал А:" );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Коэффициент усиления по расходу"), Qt::red, test::ToString( CalckGain( GraphA1 ) ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Нелинейность, %"), Qt::red, test::ToString( CalckNonlinearity( GraphA1 ) ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Гистерезис, %"), Qt::red, test::ToString( CalckHysteresis( GraphA1, GraphA2 ) ) );
    }, 2 );

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "Канал B:" );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Коэффициент усиления по расходу"), Qt::red, test::ToString( CalckGain( GraphB1 ) ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Нелинейность, %"), Qt::red, test::ToString( CalckNonlinearity( GraphB1 ) ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Гистерезис, %"), Qt::red, test::ToString( CalckHysteresis( GraphB1, GraphB2 ) ) );
    }, 2 );
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
