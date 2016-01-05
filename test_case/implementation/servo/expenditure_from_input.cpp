#include "expenditure_from_input.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
#include "test_case/implementation/test_params_servo.h"
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
    Start();
    if ( ReelControl() )
        Wait( mControlReelBits.op22_ok, mControlReelBits.op22_end );
    else
        Wait( mControlBoardBits.op12_ok, mControlBoardBits.op12_end );
    if ( IsStopped() )
        return false;

    m12Results;


    OilTemp = mTemperature.T_oil;
    auto& params = Parameters::Instance();
    if ( ReelControl() )
    {
        ///     Коэффициент усиления по расходу
        ///     1. сигнал (x_p0) при котором расход равен 0 ( а, б )
        ///     2. сигнал (x_max) при котором достигнут максимальны расход (q_max) ( максимальный расход см в параметрах ) ( а, б )
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
QJsonObject ExpeditureFromInput::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    QJsonArray a;
    foreach (Data const& d, GraphA)
    {
        a.insert( a.end(), d.Serialise() );
    }
    QJsonArray b;
    foreach (Data const& d, GraphB)
    {
        b.insert( a.end(), d.Serialise() );
    }
    obj.insert("GraphA", a );
    obj.insert("GraphB", b );
    obj.insert("Gain", Gain);
    obj.insert("Hysteresis", Hysteresis);
    obj.insert("Nonlinearity", Nonlinearity);

    return obj;
}
bool ExpeditureFromInput::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("GraphA").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            GraphA.insert( GraphA.end(), d );
    }
    QJsonArray b = obj.value("GraphB").toArray();
    foreach (QJsonValue const& v, b)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            GraphB.insert( GraphB.end(), d );
    }

    Gain = obj.value("Gain").toDouble();
    Hysteresis = obj.value("Hysteresis").toDouble();
    Nonlinearity = obj.value("Nonlinearity").toDouble();

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

        foreach ( Data const& item, GraphA )
        {
            double abs_sig = std::abs( item.Signal );
            double abs_leak = std::abs( item.Expenditure );

            if ( max_signal_a < abs_sig )
                max_signal_a = abs_sig;

            if ( max_Leak_a < abs_leak )
                max_Leak_a = abs_leak;

            dataA.push_back( QPointF( item.Signal, item.Expenditure ) );
        }
        foreach ( Data const& item, GraphB )
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
