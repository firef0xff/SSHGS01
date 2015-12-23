#include "frequency_characteristics.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
#include "../test_params_servo.h"

namespace test
{
namespace servo
{

FrequencyCharacteristics::FrequencyCharacteristics():
    test::servo::Test( "Проверка частотных характеристик", 14, 24 )
{}

bool FrequencyCharacteristics::Run()
{
    Start();
    if ( ReelControl() )
        Wait( mControlReelBits.op24_ok, mControlReelBits.op24_end );
    else
        Wait( mControlBoardBits.op14_ok, mControlBoardBits.op14_end );
    if ( IsStopped() )
        return false;

    OilTemp = mTemperature.T_oil;
    return Success();
}
void FrequencyCharacteristics::UpdateData()
{
    Test::UpdateData();

    Data d;
    if ( ReelControl() )
    {
        m24Result1.Read();
        m24Result2.Read();

        bool ready = false;
        if (ready)
        {
            for ( size_t i = 0; i < mControlReelBits.op24_number && i < m24Result1.SIGNAL_COUNT; ++i )
            {
    #warning как то что считаем
                // амплитуда считается по дельте соседних перемещений
                // фаза
                // частота с контроллера
            }
            ready = false;
            mData.push_back( d );
        }
    }
    else
    {
        m14Result1.Read();
        m14Result2.Read();

        bool ready = false;
        if (ready)
        {
            for ( size_t i = 0; i < mControlReelBits.op24_number && i < m24Result1.SIGNAL_COUNT; ++i )
            {
    #warning как то что считаем
            }
            ready = false;
            mData.push_back( d );
        }
    }
}
bool FrequencyCharacteristics::Success() const
{
    return true;
}
QJsonObject FrequencyCharacteristics::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    QJsonArray a;
    foreach (Data const& d, mData)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("Data", a );

    return obj;
}
bool FrequencyCharacteristics::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("Data").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            mData.insert( mData.end(), d );
    }
    Test::Deserialize( obj );
    return true;
}

bool FrequencyCharacteristics::Draw( QPainter& painter, QRect &free_rect ) const
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
        DrawRowCenter( rect, header_font, Qt::black, "5."+mName );
    }, 1.5 );

    res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
        DrawRowLeft( r, text_font, Qt::black, "Данные испытания предназначены для построение частотных характеристик," );
    }, 1.5 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &text_font ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, "а именно АЧХ и ФЧХ." );
    }, 1.5 );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, &params ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, test::ToString( params->PressureNominal() ) );
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

        ff0x::GraphBuilder::LinePoints data2;
        ff0x::GraphBuilder::LinePoints data2_e;

        double max_frequency = 0;
        double max_ampl = 0;
        double max_phase = 0;

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("Data").toArray();
                foreach ( QJsonValue const& v, a )
                {
                    QJsonObject o = v.toObject();

                    double phase = o.value("phase").toDouble();
                    double ampl = o.value("ampl").toDouble();
                    double frequency = o.value("frequency").toDouble();

                    data_e.push_back( QPointF( frequency, ampl ) );
                    data2_e.push_back( QPointF( frequency, phase  ) );
                }
            }
        }


        foreach ( Data const& item, mData )
        {
            double abs_sig = std::abs( item.frequency );
            double abs_ampl = std::abs( item.ampl );
            double abs_phase = std::abs( item.phase );

            if ( max_frequency < abs_sig )
                max_frequency = abs_sig;

            if ( max_ampl < abs_ampl )
                max_ampl = abs_ampl;

            if ( max_phase < abs_phase )
                max_phase = abs_phase;

            data.push_back( QPointF( item.frequency, item.ampl ) );
            data2.push_back( QPointF( item.frequency, item.phase ) );
        }

        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::GraphBuilder builder ( w, h, ff0x::GraphBuilder::PlusPlus, f );
        ff0x::GraphBuilder::GraphDataLine lines1;
        lines1.push_back( ff0x::GraphBuilder::Line(data, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data_e.empty() )
            lines1.push_back( ff0x::GraphBuilder::Line(data_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );

        ff0x::GraphBuilder::GraphDataLine lines2;
        lines2.push_back( ff0x::GraphBuilder::Line(data2, ff0x::GraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data2_e.empty() )
            lines2.push_back( ff0x::GraphBuilder::Line(data2_e, ff0x::GraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );


        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());
        DrawRowCenter( p1t, text_font, Qt::black, "АЧХ" );
        DrawRowCenter( p2t, text_font, Qt::black, "ФЧХ" );
        painter.drawPixmap( p1, builder.Draw( lines1, max_frequency * 1.25, max_ampl * 1.25, ceil(max_frequency)/10, ceil(max_ampl)/10, "Частота (Гц)", "Дб", true ) );
        painter.drawPixmap( p2, builder.Draw( lines2, max_frequency * 1.25, max_phase * 1.25, ceil(max_frequency)/10, ceil(max_phase)/10, "Частота (Гц)", "φ (гр.)", true ) );

        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );


//    free_rect.setHeight( 0 );
    return res;
}


QJsonObject FrequencyCharacteristics::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("phase", phase );
    obj.insert("ampl", ampl );
    obj.insert("frequency", frequency );

    return obj;
}
bool FrequencyCharacteristics::Data::Deserialize( QJsonObject const& obj )
{
    phase = obj.value("phase").toDouble();
    ampl = obj.value("ampl").toDouble();
    frequency = obj.value("frequency").toDouble();
    return true;
}

}//namespace servo

}//namespace test
