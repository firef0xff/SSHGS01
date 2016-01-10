#include "expenditure_from_input.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
#include "test_case/implementation/test_params_servo.h"
#include "cmath"
namespace test
{
namespace servo
{

ExpeditureFromInput::ExpeditureFromInput():
    test::servo::Test( "Зависимость расхода «к потребителю» от входного тока без нагрузки", 12, 22 ),
    Gain(0),
    Hysteresis(0)
{}

bool ExpeditureFromInput::Run()
{
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

    for ( int i = 0; i < m12Results.OPEN_REF_COUNT; ++i )
    {
        Data d;
        d.Expenditure = m12Results.open_consumption[i];
        d.Signal = m12Results.open_ref[i];
        d.PressureBP3 = m12Results.open_bp3[i];
        d.PressureBP4 = m12Results.open_bp4[i];
        d.PressureBP5 = m12Results.open_bp5[i];
        GraphA1.push_back( d );
        d.Expenditure = m12Results.close_consumption[i];
        d.Signal = m12Results.close_ref[i];
        d.PressureBP3 = m12Results.close_bp3[i];
        d.PressureBP4 = m12Results.close_bp4[i];
        d.PressureBP5 = m12Results.close_bp5[i];
    }

    OilTemp = mTemperature.T_oil;


/*
    auto& params = Parameters::Instance();
    if ( ReelControl() )
    {
        ///     Коэффициент усиления по расходу
        ///     1. сигнал (x_p0) при котором расход равен 0 ( а, б )
        ///     2. сигнал (x_max) при котором достигнут максимальны расход (q_max) ( максимальный расход см из массива ) ( а, б )
        ///     3. коффициент = Q_max/( x_max - x_p0 )
        double x_p0 = 0;
        double x_max = params.EndSgnal();
        Gain = params.MaxExpenditureA()/( x_max - x_p0 );
        Gain = params.MaxExpenditureB()/( x_max - x_p0 );
    }
    else
    {
        ///     Коэффициент усиления по расходу
        ///     1. сигнал (x_p0) при котором расход равен 0 ( а, б )
        ///     2. сигнал (x_max) при котором достигнут максимальны расход (q_max) ( максимальный расход см в параметрах ) ( а, б )
        ///     3. коффициент = Q_max/( x_max - x_p0 )
        double x_p0 = params.SignalState0();
        double x_max_a = params.SignalStateA();
        double x_max_b = -params.SignalStateB();
        Gain = params.MaxExpenditureA()/( x_max_a - x_p0 );
        Gain = params.MaxExpenditureB()/( x_max_b - x_p0 );
    }
#warning нет получения данных


///     Нелинейность
///     1. q_max - расход при максимальном управляющем сигнале x_max //из параметра
///        q_0   - расход при при нулевом управляющем сигнале x_0    // 0
///     2. вычислить коэффициент k = (q_max - q_0) / ( x_max - x_0 )
///     3. для каждого управляющего сигнала вычислить q_et[i] = k * x[i];
///     4. вычислить массив r[i] = q_et[i] - q[i];
///     5. результат равен max( r[i] ) / q_max * 100;

///     Гистерезис
///     1. max( q1[x] - q2[x] ) / q_max * 100;
*/


    return Success();
}
void ExpeditureFromInput::UpdateData()
{
    Test::UpdateData();
    m12Results.Read();
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
    int pos_max_exp = 0;
    int pos_min_signal = 0;
    for ( int i = 0; i < data.size(); ++i )
    {
        if ( data[i].Expenditure > data[pos_max_exp].Expenditure )
            pos_max_exp = i;
        if ( round( data[i].Expenditure ) == 0.0 )
        {
            if ( fabs( data[i].Signal ) > fabs( data[pos_min_signal].Signal ) )
                pos_min_signal = i;
        }
    }
/// Коэффициент усиления по расходу
///     1. сигнал (x_p0) при котором расход равен 0 ( а, б )
///     2. сигнал (x_max) при котором достигнут максимальны расход (q_max) ( максимальный расход см из массива ) ( а, б )
///     3. коффициент = Q_max/( x_max - x_p0 )
    double Q = data[pos_max_exp].Expenditure;
    double x_max = data[pos_max_exp].Signal;
    double x_p0 = data[pos_min_signal].Signal;
    return Q/( x_max - x_p0 );
}
double CalckNonlinearity( ExpeditureFromInput::DataSet const& data )
{
    int pos_max_exp = 0;
    int pos_min_signal = 0;
    for ( int i = 0; i < data.size(); ++i )
    {
        if ( data[i].Expenditure > data[pos_max_exp].Expenditure )
            pos_max_exp = i;
        if ( round( data[i].Expenditure ) == 0.0 )
        {
            if ( fabs( data[i].Signal ) > fabs( data[pos_min_signal].Signal ) )
                pos_min_signal = i;
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

    double k = (q_max - q_0) / ( x_max - x_0 );

    double max_r;
    for ( int i = 0; i < data.size(); ++i )
    {
        double const& x = data[i].Signal;
        double const& q = data[i].Expenditure;

        double r = k*x - q;
        if ( !i )
            max_r = r;
        else if ( r > max_r )
            max_r = r;
    }
    return max_r / q_max * 100;
}
double CalckHysteresis( ExpeditureFromInput::DataSet const& data )
{
///     Гистерезис
///     1. max( q1[x] - q2[x] ) / q_max * 100;
    return 0;
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

bool ExpeditureFromInput::Draw( QPainter& painter, QRect &free_rect ) const
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
    [ this, &painter, &text_font, &DrawRowCenter, &metrix ]( QRect const& rect )
    {
        painter.save();

        ff0x::GraphBuilder::LinePoints dataA;
        ff0x::GraphBuilder::LinePoints dataA_e;

        ff0x::GraphBuilder::LinePoints dataB;
        ff0x::GraphBuilder::LinePoints dataB_e;

        double max_signal_a = 0;
        double max_Leak_a = 0;

        double max_signal_b = 0;
        double max_Leak_b = 0;

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("GraphA").toArray();
                foreach ( QJsonValue const& v, a )
                {
                    QJsonObject o = v.toObject();
                    dataA_e.push_back( QPointF( o.value("Signal").toDouble(), o.value("Expenditure").toDouble() ) );
                }
                QJsonArray b = obj.value("data").toObject().value("GraphB").toArray();
                foreach ( QJsonValue const& v, b )
                {
                    QJsonObject o = v.toObject();
                    dataB_e.push_back( QPointF( o.value("Signal").toDouble(), o.value("Expenditure").toDouble() ) );
                }
            }
        }

        foreach ( Data const& item, GraphA1 )
        {
            double abs_sig = std::abs( item.Signal );
            double abs_leak = std::abs( item.Expenditure );

            if ( max_signal_a < abs_sig )
                max_signal_a = abs_sig;

            if ( max_Leak_a < abs_leak )
                max_Leak_a = abs_leak;

            dataA.push_back( QPointF( item.Signal, item.Expenditure ) );
        }
        foreach ( Data const& item, GraphB1 )
        {
            double abs_sig = std::abs( item.Signal );
            double abs_leak = std::abs( item.Expenditure );

            if ( max_signal_b < abs_sig )
                max_signal_b = abs_sig;

            if ( max_Leak_b < abs_leak )
                max_Leak_b = abs_leak;

            dataB.push_back( QPointF( item.Signal, item.Expenditure ) );
        }

        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::GraphBuilder builder ( w, h, ff0x::GraphBuilder::PlusPlus, f );
        ff0x::GraphBuilder::GraphDataLine lines_a;
        lines_a.push_back( ff0x::GraphBuilder::Line(dataA, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !dataA_e.empty() )
            lines_a.push_back( ff0x::GraphBuilder::Line(dataA_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        ff0x::GraphBuilder::GraphDataLine lines_b;
        lines_b.push_back( ff0x::GraphBuilder::Line(dataB, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !dataB_e.empty() )
            lines_b.push_back( ff0x::GraphBuilder::Line(dataB_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );


        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());
        DrawRowCenter( p1t, text_font, Qt::black, "P->A" );
        DrawRowCenter( p2t, text_font, Qt::black, "P->B" );
        painter.drawPixmap( p1, builder.Draw( lines_a, max_signal_a * 1.25, max_Leak_a * 1.25, ceil(max_signal_a)/10, ceil(max_Leak_a)/10, "Опорный сигнал", "Расход (л/мин)", true ) );
        painter.drawPixmap( p2, builder.Draw( lines_b, max_signal_b * 1.25, max_Leak_b * 1.25, ceil(max_signal_b)/10, ceil(max_Leak_b)/10, "Опорный сигнал", "Расход (л/мин)", true ) );

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Коэффициент усиления по расходу"), Qt::red, test::ToString( Gain ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Нелинейность, %"), Qt::red, test::ToString( Nonlinearity ) );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Гистерезис, %"), Qt::red, test::ToString( Hysteresis ) );
    }, 2 );

    return res;
}

QJsonObject ExpeditureFromInput::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("Signal", Signal );
    obj.insert("Expenditure", Expenditure );

    obj.insert("PressureBP3", PressureBP3 );
    obj.insert("PressureBP4", PressureBP4 );
    obj.insert("PressureBP5", PressureBP5 );

    return obj;
}
bool ExpeditureFromInput::Data::Deserialize( QJsonObject const& obj )
{
    Signal = obj.value("Signal").toDouble();
    Expenditure = obj.value("Expenditure").toDouble();

    PressureBP3 = obj.value("PressureBP3").toDouble();
    PressureBP4 = obj.value("PressureBP4").toDouble();
    PressureBP5 = obj.value("PressureBP5").toDouble();

    return true;
}

}//namespace servo

}//namespace test
