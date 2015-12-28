#include "frequency_characteristics.h"
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

FrequencyCharacteristics::FrequencyCharacteristics():
    test::servo::Test( "Проверка частотных характеристик", 14, 24 )
{}

bool FrequencyCharacteristics::Run()
{
    mData.clear();
    mSource1.clear();
    mSource2.clear();
    mSource3.clear();
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

//    Data d;

//    auto f_S = []( double expenditure )
//    {
//        double r1 = 0, r2 = 0;
//        if ( expenditure < 30 )
//        {
//            r1 = 25.0/2.0;
//            r2 = 18.0/2.0;
//        }
//        else
//        {
//            r1 = 63.0/2.0;
//            r2 = 36.0/2.0;
//        }
//        return 2 *3.14 * ( r1*r1 - r2*r2 );
//    };
//    double K = 0.00006 * f_S( test::servo::Parameters::Instance().DefaultExpenditure() );
//    double time_period = 1/1000; //время между замерами данных с

    if ( ReelControl() )
    {
        if (!mControlReelBits.op24_ready )
            return;

        std::this_thread::sleep_for(std::chrono::seconds(5));
        mControlReelBits.Read();
        m24Result1.Read();
        m24Result2.Read();
        m1525Counts.Read();

        Source *src = nullptr;
        if ( m1525Counts.OP15_25_Opor_1 )
            src = &mSource1;
        if ( m1525Counts.OP15_25_Opor_2 )
            src = &mSource2;
        if ( m1525Counts.OP15_25_Opor_3 )
            src = &mSource3;
        if ( !src )
            return;

        if ( src->find( mControlReelBits.op24_frequency ) != src->end() )
            return;

        DataSet data;
        for ( int i = 0; i < m1525Counts.OP15_25_count && i < m24Result1.SIGNAL_COUNT; ++i )
        {
            ArrData item;
            item.position = m24Result2.coordinate[i];
            item.signal = m24Result1.signal[i];
            data.push_back( item );
        }
        src->insert( SourceItem( mControlReelBits.op24_frequency, data ) );


//            std::vector< double > expenditure;
//            for ( size_t i = 0; i < mControlReelBits.op24_number && i < m24Result1.SIGNAL_COUNT; ++i )
//            {
//                double ext = 0; // пройденное расстояние в мм
//                if ( i > 1 )
//                    ext = m24Result2.coordinate[i] - m24Result2.coordinate[i - 1];

//                double speed = ext / time_period; //скорость движения мм/сек
//                if ( i > 1 )
//                    expenditure.push_back( speed * K ); // расход л/мин
//            }
//            //определим амплитуду
//            size_t q_min = 0;
//            size_t q_max = 0;
//            if ( !expenditure.empty() )
//            {
//                q_max = 0;
//                q_min = 0;
//            }
//            for ( size_t i = 0; i < expenditure.size(); ++i )
//            {
//                if ( expenditure[i] > expenditure[q_max] )
//                    q_max = i;
//                if ( expenditure[i] < expenditure[q_min] )
//                    q_min = i;
//            }
//            size_t s_min = 0;
//            size_t s_max = 0;

//            for ( size_t i = 0; i < mControlReelBits.op24_number && i < m24Result1.SIGNAL_COUNT; ++i )
//            {
//                if ( m24Result1.signal[i] > m24Result1.signal[s_max] )
//                    s_max = i;
//                if ( m24Result1.signal[i] < m24Result1.signal[s_min] )
//                    s_min = i;
//            }

//            double Tsm = q_max - s_max - 1;
//            double T = 1/mControlReelBits.op24_frequency;
//            double fi = -Tsm/T*360;

//            d.frequency = mControlReelBits.op24_frequency;
//            d.ampl = q_max - q_min;
//            d.phase = fi;
//            mData.push_back( d );
        cpu::CpuMemory::Instance().DB31.SendContinue();

    }
    else
    {
#warning синхронизировать
        m14Result1.Read();
        m14Result2.Read();

        if (mControlBoardBits.op14_ready)
        {
//            std::vector< double > expenditure;
//            for ( size_t i = 0; i < mControlBoardBits.op14_number && i < m14Result1.SIGNAL_COUNT; ++i )
//            {
//                double ext = 0; // пройденное расстояние в мм
//                if ( i > 1 )
//                    ext = m14Result2.coordinate[i] - m14Result2.coordinate[i - 1];

//                double speed = ext / time_period; //скорость движения мм/сек
//                if ( i > 1 )
//                    expenditure.push_back( speed * K ); // расход л/мин
//            }
//            //определим амплитуду
//            size_t q_min = 0;
//            size_t q_max = 0;
//            if ( !expenditure.empty() )
//            {
//                q_max = 0;
//                q_min = 0;
//            }
//            for ( size_t i = 0; i < expenditure.size(); ++i )
//            {
//                if ( expenditure[i] > expenditure[q_max] )
//                    q_max = i;
//                if ( expenditure[i] < expenditure[q_min] )
//                    q_min = i;
//            }
//            size_t s_min = 0;
//            size_t s_max = 0;

//            for ( size_t i = 0; i < mControlBoardBits.op14_number && i < m14Result1.SIGNAL_COUNT; ++i )
//            {
//                if ( m14Result1.signal[i] > m14Result1.signal[s_max] )
//                    s_max = i;
//                if ( m14Result1.signal[i] < m14Result1.signal[s_min] )
//                    s_min = i;
//            }

//            double Tsm = q_max - s_max - 1;
//            double T = 1/mControlBoardBits.op14_frequency;
//            double fi = -Tsm/T*360;

//            d.frequency = mControlBoardBits.op14_frequency;
//            d.ampl = q_max - q_min;
//            d.phase = fi;
//            mData.push_back( d );
            cpu::CpuMemory::Instance().DB31.SendContinue();
        }
    }
}
bool FrequencyCharacteristics::Success() const
{
    return true;
}

QJsonArray ToJson( FrequencyCharacteristics::Source const& in_src )
{
    QJsonArray source;
    for ( auto it = in_src.begin(), end = in_src.end(); it != end; ++it )
    {
        double key = it->first;
        FrequencyCharacteristics::DataSet const& data = it->second;
        QJsonArray data_set;
        for ( auto it2 = data.begin(), end2 = data.end(); it2!=end2; ++it2)
        {
            FrequencyCharacteristics::ArrData const& lnk = *it2;
            QJsonObject item;
            item.insert( "position", lnk.position );
            item.insert( "signal", lnk.signal );
            data_set.insert( data_set.end(), item );
        }
        QJsonObject src;
        src.insert("key", key);
        src.insert("data_set", data_set);
        source.insert( source.end(), src );
    }

    return source;
}

FrequencyCharacteristics::Source FromJson( QJsonArray arr )
{
    FrequencyCharacteristics::Source res;
    foreach (QJsonValue const& v, arr)
    {
        QJsonObject src = v.toObject();
        double key = src.value("key").toDouble();
        FrequencyCharacteristics::DataSet ds;
        QJsonArray data_set = src.value("data_set").toArray();
        foreach ( QJsonValue const& v_item, data_set )
        {
            QJsonObject item = v_item.toObject();
            FrequencyCharacteristics::ArrData it;
            it.position = item.value("position").toDouble();
            it.signal = item.value("signal").toDouble();
            ds.push_back( it );
        }
        res.insert( FrequencyCharacteristics::SourceItem( key, ds ) );
    }
    return std::move( res );
}

QJsonObject FrequencyCharacteristics::Serialise() const
{
    QJsonObject obj = Test::Serialise();
//    QJsonArray a;
//    foreach (Data const& d, mData)
//    {
//        a.insert( a.end(), d.Serialise() );
//    }
//    obj.insert("Data", a );

    obj.insert( "Source1", ToJson( mSource1 ) );
    obj.insert( "Source2", ToJson( mSource2 ) );
    obj.insert( "Source3", ToJson( mSource3 ) );

    return obj;
}
bool FrequencyCharacteristics::Deserialize( QJsonObject const& obj )
{
    mData.clear();
    mSource1.clear();
    mSource2.clear();
    mSource3.clear();
//    QJsonArray a = obj.value("Data").toArray();
//    foreach (QJsonValue const& v, a)
//    {
//        Data d;
//        if ( d.Deserialize( v.toObject() ) )
//            mData.insert( mData.end(), d );
//    }

    mSource1 = FromJson( obj.value("Source1").toArray() );
    mSource2 = FromJson( obj.value("Source2").toArray() );
    mSource3 = FromJson( obj.value("Source3").toArray() );

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
