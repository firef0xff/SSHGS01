#include "frequency_characteristics.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
#include "../test_params_servo.h"
#include <thread>
//#define DEBUG
namespace test
{
namespace servo
{
namespace
{

#define PI 3.14159265
FrequencyCharacteristics::Source Build()
{
    FrequencyCharacteristics::Source data;

    for ( int i = 1; i < 100; ++i )
    {
        FrequencyCharacteristics::DataSet data_item;
        for ( int j = 0; j <= 360; ++j )
        {
            FrequencyCharacteristics::ArrData coord;
            coord.signal = sin( j * PI / 180.0 ) * 3;
            coord.position = sin( ( j + 180 + double(i)/2 ) * PI / 180.0  ) * 2.5/(i*i/1600.0);
            data_item.push_back( coord );
        }
        data.insert( FrequencyCharacteristics::SourceItem( i, data_item ) );
    }
    return std::move( data );
}

}

namespace
{

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

double CalckAmpl( FrequencyCharacteristics::DataSet const& data )
{
    double ampl = 0;
    std::vector< double > expenditure;
    for ( size_t i = 0; i < data.size(); ++i )
    {
        if ( !i )
            expenditure.push_back(0);
        else
            expenditure.push_back( (data[i].position - data[i-1].position) );
    }

    if ( !expenditure.empty() )
    {
        int max = 0;
        int min = 0;
        for ( size_t i = 0; i < expenditure.size(); ++i )
        {
            if ( expenditure[ max ] < expenditure[i] )
                max = i;
            if ( expenditure[ min ] > expenditure[i] )
                min = i;
        }

        ampl = expenditure[ max ] - expenditure[ min ];
    }

    return ampl;
}

ff0x::NoAxisGraphBuilder::LinePoints ProcessAFC( FrequencyCharacteristics::Source const& src, QPointF& x_range, QPointF& y_range )
{
    auto f_S = []( double expenditure )
    {
        double r1 = 0, r2 = 0;
        if ( expenditure < 30 )
        {
            r1 = 25.0/2.0;
            r2 = 18.0/2.0;
        }
        else
        {
            r1 = 63.0/2.0;
            r2 = 36.0/2.0;
        }
        return 2 *3.14 * ( r1*r1 - r2*r2 );
    };
    double K = 0.00006 * f_S( test::servo::Parameters::Instance().DefaultExpenditure() );
    double time_period = 1.0/1000.0; //время между замерами данных с

    ff0x::NoAxisGraphBuilder::LinePoints result;
    double min_ampl;
    for ( auto it = src.begin(), end = src.end(); it != end; ++it  )
    {
        QPointF point;
        point.setX( it->first );

        double ampl = CalckAmpl( it->second ) *K / time_period;

        if ( it == src.begin() )
            min_ampl = ampl;

        if ( min_ampl != 0 && ampl/min_ampl != 0 )
        {
            point.setY( 10.0*log10( ampl / min_ampl) );
        }
        else
            point.setY(0);

        if ( it == src.begin() )
        {
            x_range.setX(point.x());
            x_range.setY(point.x());
            y_range.setX(point.y());
            y_range.setY(point.y());
        }
        else
        {
            if ( x_range.x() < point.x() )
                x_range.setX( point.x() );
            if ( x_range.y() > point.x() )
                x_range.setY( point.x() );

            if ( y_range.x() < point.y() )
                y_range.setX( point.y() );
            if ( y_range.y() > point.y() )
                y_range.setY( point.y() );
        }

#ifndef DEBUG
        if ( fabs( point.y() ) > 15 )
            break;
#endif
        result.push_back( point );
    }
    return std::move( result );
}


class GrapfInfo
{
public:
    struct PointInfo
    {
    public:
        enum PointType
        {
            Unset,
            Min,
            Max
        };
        PointInfo():
            point(),
            type(Unset)
        {}
        PointInfo( QPointF const& p, PointType t ):
            point( p ),
            type( t )
        {}

        QPointF point;
        PointType type;
    };
    typedef QVector< PointInfo > Data;

    GrapfInfo( QVector<QPointF> const& data )
    {
        int old_way = 0;
        QPointF curr_p;
        for ( int i = 0; i < data.size(); ++i )
        {
            if ( !i )
                continue;

            curr_p = data[i];
            QPointF const& prev_p = data[i - 1];
            double delta = curr_p.y() - prev_p.y();
            int way = delta > 0 ? 1 : delta < 0 ? -1 : 0;
            if ( way != old_way )
            {
                if ( way > 0 )
                    Info.push_back(PointInfo( curr_p, PointInfo::Min ));
                else if ( way < 0 )
                    Info.push_back(PointInfo( curr_p, PointInfo::Max ));
                old_way = way;
            }
        }
        if ( old_way )
        {
            if ( old_way < 0 )
                Info.push_back(PointInfo( curr_p, PointInfo::Min ));
            else if ( old_way > 0 )
                Info.push_back(PointInfo( curr_p, PointInfo::Max ));
        }
    }

    Data const& GetInfo() const
    {
        return Info;
    }
private:
    Data Info;
};

class SinusAnaliser
{
public:
    struct PointInfo
    {
    public:
        enum PointType
        {
            Unset,
            Min,
            Max,
            Start,
            Stop
        };
        PointInfo():
            point(),
            type(Unset)
        {}
        PointInfo( QPointF const& p, PointType t ):
            point( p ),
            type( t )
        {}

        QPointF point;
        PointType type;
    };
    typedef QVector< PointInfo > Data;

    SinusAnaliser( QVector<QPointF> const& data )
    {
        int old_way = 0;
        QPointF curr_p;
        QPointF max_p;
        QPointF min_p;
        for ( int i = 0; i < data.size(); ++i )
        {
            curr_p = data[i];
            int way = curr_p.y() > 0 ? 1 : curr_p.y() < 0 ? -1 : old_way;
            if ( way > 0 )
            {
                if ( max_p.y() < curr_p.y() )
                    max_p = curr_p;
            }
            if ( way < 0 )
            {
                if ( min_p.y() > curr_p.y() )
                    min_p = curr_p;
            }
            if ( way != old_way && old_way )
            {
                if ( way > 0 )
                {
                    Info.push_back(PointInfo( min_p, PointInfo::Min ));
                    min_p = QPointF();
                }
                else if ( way < 0 )
                {
                    Info.push_back(PointInfo( max_p, PointInfo::Max ));
                    max_p = QPointF();
                }
            }
            old_way = way;
        }
        if ( old_way )
        {
            if ( old_way < 0 )
                Info.push_back(PointInfo( min_p, PointInfo::Min ));
            else if ( old_way > 0 )
                Info.push_back(PointInfo( max_p, PointInfo::Max ));
        }
    }

    Data const& GetInfo() const
    {
        return Info;
    }
private:
    Data Info;
};

double CalckFi( FrequencyCharacteristics::DataSet const& data, double frequency )
{
    QVector< QPointF > speed;
    QVector< QPointF > signal;
    for ( size_t i = 0; i < data.size(); ++i )
    {
        signal.push_back( QPointF( i, data[i].signal ) );
        if ( !i )
            speed.push_back( QPointF( i, data[i + 1].position - data[i].position ) );
        else
            speed.push_back( QPointF( i, data[i].position - data[i-1].position ) );
    }

    SinusAnaliser info_speed( speed );
    SinusAnaliser info_signal( signal );

//        ищем координату первого максимума сигнала
    SinusAnaliser::Data const& inf = info_signal.GetInfo();
    double signal_max_time = 0;
    for ( int i = 0; i < inf.size(); ++i )
    {
        SinusAnaliser::PointInfo const& point = inf[i];
        if ( point.type == SinusAnaliser::PointInfo::Max )
        {
            signal_max_time = point.point.x();
            break;
        }
    }

//        ищем координату первого максимума сигнала
    SinusAnaliser::Data const& inf_sg = info_speed.GetInfo();
    double speed_max_time = 0;
    for ( int i = 0; i < inf_sg.size(); ++i )
    {
        SinusAnaliser::PointInfo const& point = inf_sg[i];
        if ( point.type == SinusAnaliser::PointInfo::Max && point.point.x() >= signal_max_time )
        {
            speed_max_time = point.point.x();
            break;
        }
    }

    double Tsm = speed_max_time - signal_max_time;
    double T = 1/frequency;
    double fi = -Tsm/(1000*T)*360;

    return fi;
}

ff0x::NoAxisGraphBuilder::LinePoints ProcessPFC( FrequencyCharacteristics::Source const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;
    for ( auto it = src.begin(), end = src.end(); it != end; ++it  )
    {
        QPointF point;
        point.setX( it->first );
        point.setY( CalckFi( it->second, it->first ) );

        if ( it == src.begin() )
        {
            x_range.setX(point.x());
            x_range.setY(point.x());
            y_range.setX(point.y());
            y_range.setY(point.y());
        }
        else
        {
            if ( x_range.x() < point.x() )
                x_range.setX( point.x() );
            if ( x_range.y() > point.x() )
                x_range.setY( point.x() );

            if ( y_range.x() < point.y() )
                y_range.setX( point.y() );
            if ( y_range.y() > point.y() )
                y_range.setY( point.y() );
        }
#ifndef DEBUG
        if ( fabs( point.y() ) > 90 )
            break;
#endif
        result.push_back( point );
    }
    return std::move( result );
}

ff0x::NoAxisGraphBuilder::LinePoints ProcessDebug1( FrequencyCharacteristics::Source const& src, int i , QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;
    int k = 0;
    for ( auto it = src.begin(), end = src.end(); it != end; ++it  )
    {
        if ( i == k )
        {
            FrequencyCharacteristics::DataSet const& lnk = it->second;
            for ( size_t j = 0; j < lnk.size(); ++j )
            {
                QPointF point;
                point.setX( j );
                point.setY( lnk[j].signal );
                if ( j == 0 )
                {
                    x_range.setX(point.x());
                    x_range.setY(point.x());
                    y_range.setX(point.y());
                    y_range.setY(point.y());
                }
                else
                {
                    if ( x_range.x() < point.x() )
                        x_range.setX( point.x() );
                    if ( x_range.y() > point.x() )
                        x_range.setY( point.x() );

                    if ( y_range.x() < point.y() )
                        y_range.setX( point.y() );
                    if ( y_range.y() > point.y() )
                        y_range.setY( point.y() );
                }
                result.push_back( point );
            }
            break;
        }
        ++k;
    }
    return std::move( result );
}
ff0x::NoAxisGraphBuilder::LinePoints ProcessDebug2( FrequencyCharacteristics::Source const& src, int i , QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;
    int k = 0;
    for ( auto it = src.begin(), end = src.end(); it != end; ++it  )
    {
        if ( i == k )
        {
            FrequencyCharacteristics::DataSet const& lnk = it->second;
            for ( size_t j = 0; j < lnk.size(); ++j )
            {
                QPointF point;
                point.setX( j );
                point.setY( lnk[j].position );
                if ( j == 0 )
                {
                    x_range.setX(point.x());
                    x_range.setY(point.x());
                    y_range.setX(point.y());
                    y_range.setY(point.y());
                }
                else
                {
                    if ( x_range.x() < point.x() )
                        x_range.setX( point.x() );
                    if ( x_range.y() > point.x() )
                        x_range.setY( point.x() );

                    if ( y_range.x() < point.y() )
                        y_range.setX( point.y() );
                    if ( y_range.y() > point.y() )
                        y_range.setY( point.y() );
                }
                result.push_back( point );
            }
            break;
        }
        ++k;
    }
    return std::move( result );
}

ff0x::NoAxisGraphBuilder::LinePoints ProcessDebug3( FrequencyCharacteristics::Source const& src, int i , QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;
    int k = 0;
    for ( auto it = src.begin(), end = src.end(); it != end; ++it  )
    {
        if ( i == k )
        {
            FrequencyCharacteristics::DataSet const& lnk = it->second;
            for ( size_t j = 0; j < lnk.size(); ++j )
            {
                QPointF point;
                point.setX( j );
                if ( !j )
                    point.setY( lnk[j + 1].position - lnk[j].position );
                else
                    point.setY( lnk[j].position - lnk[j-1].position );
                if ( j == 0 )
                {
                    x_range.setX(point.x());
                    x_range.setY(point.x());
                    y_range.setX(point.y());
                    y_range.setY(point.y());
                }
                else
                {
                    if ( x_range.x() < point.x() )
                        x_range.setX( point.x() );
                    if ( x_range.y() > point.x() )
                        x_range.setY( point.x() );

                    if ( y_range.x() < point.y() )
                        y_range.setX( point.y() );
                    if ( y_range.y() > point.y() )
                        y_range.setY( point.y() );
                }
                result.push_back( point );
            }
            break;
        }
        ++k;
    }
    return std::move( result );
}

ff0x::NoAxisGraphBuilder::LinePoints ProcessDebug4( FrequencyCharacteristics::Source const& src, int i , QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;
    int k = 0;
    for ( auto it = src.begin(), end = src.end(); it != end; ++it  )
    {
        if ( i == k )
        {
            FrequencyCharacteristics::DataSet const& lnk = it->second;
            QVector<QPointF> data;
            for ( size_t j = 0; j < lnk.size(); ++j )
            {
                QPointF point;
                point.setX( j );
                if ( !j )
                    point.setY( lnk[j + 1].position - lnk[j].position );
                else
                    point.setY( lnk[j].position - lnk[j-1].position );
                data.push_back( point );
            }

            SinusAnaliser inf( data );
            for ( size_t j = 0; j < inf.GetInfo().size(); ++j )
            {
                QPointF point = inf.GetInfo()[j].point;

                if ( j == 0 )
                {
                    x_range.setX(point.x());
                    x_range.setY(point.x());
                    y_range.setX(point.y());
                    y_range.setY(point.y());
                }
                else
                {
                    if ( x_range.x() < point.x() )
                        x_range.setX( point.x() );
                    if ( x_range.y() > point.x() )
                        x_range.setY( point.x() );

                    if ( y_range.x() < point.y() )
                        y_range.setX( point.y() );
                    if ( y_range.y() > point.y() )
                        y_range.setY( point.y() );
                }
                result.push_back( point );
            }
            break;
        }
        ++k;
    }
    return std::move( result );
}


}//namespace


FrequencyCharacteristics::FrequencyCharacteristics():
    test::servo::Test( "Проверка частотных характеристик", 14, 24 )
{
#ifdef DEMO
    mSource1 = Build();
#endif
}

bool FrequencyCharacteristics::Run()
{
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

    OilTemp = round( mTemperature.T_oil *100)/100;
    return Success();
}
void FrequencyCharacteristics::UpdateData()
{
    Test::UpdateData();

    std::this_thread::sleep_for(std::chrono::seconds(5));
    bool *ready = 0;
    float *frequency = 0;


    if ( ReelControl() )
    {
        ready = &mControlReelBits.op24_ready;
        frequency = &mControlReelBits.op24_frequency;
    }
    else
    {
        ready = &mControlBoardBits.op14_ready;
        frequency = &mControlBoardBits.op14_frequency;
    }


    if (!*ready )
        return;
    m14Result1.Read();
    m14Result2.Read();
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

    if ( src->find( *frequency ) != src->end() )
        return;

    DataSet data;
    for ( int i = 0; i < m1525Counts.OP15_25_count && i < m14Result1.SIGNAL_COUNT; ++i )
    {
        ArrData item;
        item.position = m14Result2.coordinate[i];
        item.signal = m14Result1.signal[i];
        data.push_back( item );
    }

#ifndef DEBUG
    if ( fabs( CalckFi( data, *frequency ) ) > 90.0 || fabs( CalckAmpl( data ) ) < 0.01 )
        cpu::CpuMemory::Instance().DB31.SendNextAmp();
    else
#endif
        src->insert( SourceItem( *frequency, data ) );
    cpu::CpuMemory::Instance().DB31.SendContinue();

}
bool FrequencyCharacteristics::Success() const
{
    return true;
}


QJsonObject FrequencyCharacteristics::Serialise() const
{
    QJsonObject obj = Test::Serialise();

    obj.insert( "Source1", ToJson( mSource1 ) );
    obj.insert( "Source2", ToJson( mSource2 ) );
    obj.insert( "Source3", ToJson( mSource3 ) );

    return obj;
}
bool FrequencyCharacteristics::Deserialize( QJsonObject const& obj )
{
    mSource1 = FromJson( obj.value("Source1").toArray() );
    mSource2 = FromJson( obj.value("Source2").toArray() );
    mSource3 = FromJson( obj.value("Source3").toArray() );

    Test::Deserialize( obj );
    return true;
}

bool FrequencyCharacteristics::Draw( QPainter& painter, QRect &free_rect, const QString &compare_width ) const
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
#ifdef DEBUG
    auto DEBUG_DATA = [&]( Source const& s)
    {
        int size = s.size();
        for ( int i = 0; i < size; ++i )
        {
            res = DrawLine( num, free_rect, text_font,
            [ this, &painter, &text_font, &DrawRowCenter, &metrix, i, &s ]( QRect const& rect )
            {
                painter.save();

                ff0x::GraphBuilder::LinePoints data;
                ff0x::GraphBuilder::LinePoints data2;
                ff0x::GraphBuilder::LinePoints data3;

                QPointF x_range_1;
                QPointF y_range_1;

                QPointF x_range_2;
                QPointF y_range_2;

                QPointF x_range_3;
                QPointF y_range_3;

                data = ProcessDebug2( s, i, x_range_1, y_range_1 );
                data2 = ProcessDebug3( s, i, x_range_2, y_range_2 );
                data3 = ProcessDebug4( s, i, x_range_3, y_range_3 );

                QFont f = text_font;
                f.setPointSize( 6 );
                int w = (rect.height() - metrix.height())*0.98;
                int h = (rect.height() - metrix.height())*0.98;

                ff0x::NoAxisGraphBuilder builder ( w, h, f );
                ff0x::NoAxisGraphBuilder::GraphDataLine lines1;
                ff0x::NoAxisGraphBuilder::GraphDataLine lines2;
                lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data, ff0x::NoAxisGraphBuilder::LabelInfo( "", Qt::blue ) ) );
    //            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data2, ff0x::NoAxisGraphBuilder::LabelInfo( "", Qt::red ) ) );
                lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2, ff0x::NoAxisGraphBuilder::LabelInfo( "", Qt::red ) ) );
                lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data3, ff0x::NoAxisGraphBuilder::LabelInfo( "", Qt::blue ) ) );

                QRect p1(rect.left(), rect.top(), w, h );
                QRect p2(rect.right() - w, rect.top(), w, h );
                QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
                QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());

                DrawRowCenter( p1t, text_font, Qt::black, "результат " + QString::number( i + 1  ) );
                {
                    QPointF x_range;
                    QPointF y_range;
                    double x_step = 0;
                    double y_step = 0;

    //                ff0x::DataLength( x_range_1, x_range_2, true, x_range, x_step );
    //                ff0x::DataLength( y_range_1, y_range_2, true, y_range, y_step );

                    ff0x::DataLength( x_range_1, x_range, x_step );
                    ff0x::DataLength( y_range_1, y_range, y_step );

                    if ( y_range.x() - y_range.y() == 0 )
                    {
                        y_range.setX( 1 );
                        y_range.setY( -1 );
                    }
                    painter.drawPixmap( p1, builder.Draw( lines1, x_range, y_range, x_step, y_step, "x", "y", true ) );
                }
                DrawRowCenter( p2t, text_font, Qt::black, "результат " + QString::number( i + 1  ) );
                {
                    QPointF x_range;
                    QPointF y_range;
                    double x_step = 0;
                    double y_step = 0;

                    ff0x::DataLength( x_range_2, x_range_3, true, x_range, x_step );
                    ff0x::DataLength( y_range_2, y_range_3, true, y_range, y_step );

//                    ff0x::DataLength( x_range_3, x_range, x_step );
//                    ff0x::DataLength( y_range_3, y_range, y_step );

                    if ( y_range.x() - y_range.y() == 0 )
                    {
                        y_range.setX( 1 );
                        y_range.setY( -1 );
                    }
                    painter.drawPixmap( p2, builder.Draw( lines2, x_range, y_range, x_step, y_step, "x", "y", true ) );
                }

                painter.restore();
            }, 1, free_rect.width()/2 + metrix.height()  );
        }
    };

    DEBUG_DATA( mSource1 );
    DEBUG_DATA( mSource2 );
    DEBUG_DATA( mSource3 );
#endif

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

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("mSource1").toArray();
                data1_e = ProcessAFC( FromJson( a ), x_range_1e, y_range_1e );
                data2_e = ProcessPFC( FromJson( a ), x_range_2e, y_range_2e );
            }
        }
        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("mSource1").toArray();
                data1_e2 = ProcessAFC( FromJson( a ), x_range_1e2, y_range_1e2 );
                data2_e2 = ProcessPFC( FromJson( a ), x_range_2e2, y_range_2e2 );
            }
        }

        data1 = ProcessAFC( mSource1, x_range_1, y_range_1 );
        data2 = ProcessPFC( mSource1, x_range_2, y_range_2 );


        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines1;
        lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data1_e.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !data1_e2.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        ff0x::NoAxisGraphBuilder::GraphDataLine lines2;
        lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data2_e.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data2_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !data2_e2.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data2_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());

        DrawRowCenter( p1t, text_font, Qt::black, "АЧХ. Амплитуда 1" );
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
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p1, builder.Draw( lines1, x_range, y_range, x_step, y_step, "Частота (Гц)", "Дб", true ) );
        }

        DrawRowCenter( p2t, text_font, Qt::black, "ФЧХ. Амплитуда 1" );
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
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p2, builder.Draw( lines2, x_range, y_range, x_step, y_step, "Частота (Гц)", "φ (гр.)", true ) );
        }
        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );

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

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("mSource2").toArray();
                data1_e = ProcessAFC( FromJson( a ), x_range_1e, y_range_1e );
                data2_e = ProcessPFC( FromJson( a ), x_range_2e, y_range_2e );
            }
        }

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("mSource2").toArray();
                data1_e2 = ProcessAFC( FromJson( a ), x_range_1e2, y_range_1e2 );
                data2_e2 = ProcessPFC( FromJson( a ), x_range_2e2, y_range_2e2 );
            }
        }

        data1 = ProcessAFC( mSource2, x_range_1, y_range_1 );
        data2 = ProcessPFC( mSource2, x_range_2, y_range_2 );


        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines1;
        lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data1_e.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !data1_e2.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        ff0x::NoAxisGraphBuilder::GraphDataLine lines2;
        lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data2_e.empty() )
            lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !data2_e2.empty() )
            lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );


        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());

        DrawRowCenter( p1t, text_font, Qt::black, "АЧХ. Амплитуда 2" );
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
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p1, builder.Draw( lines1, x_range, y_range, x_step, y_step, "Частота (Гц)", "Дб", true ) );
        }

        DrawRowCenter( p2t, text_font, Qt::black, "ФЧХ. Амплитуда 2" );
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
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p2, builder.Draw( lines2, x_range, y_range, x_step, y_step, "Частота (Гц)", "φ (гр.)", true ) );
        }
        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );

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

        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromEtalone().value( test::CURRENT_PARAMS->ModelId()).toObject().value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("mSource3").toArray();
                data1_e = ProcessAFC( FromJson( a ), x_range_1e, y_range_1e );
                data2_e = ProcessPFC( FromJson( a ), x_range_2e, y_range_2e );
            }
        }
        //поиск данных теста
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == mId )
            {
                QJsonArray a = obj.value("data").toObject().value("mSource3").toArray();
                data1_e2 = ProcessAFC( FromJson( a ), x_range_1e2, y_range_1e2 );
                data2_e2 = ProcessPFC( FromJson( a ), x_range_2e2, y_range_2e2 );
            }
        }

        data1 = ProcessAFC( mSource3, x_range_1, y_range_1 );
        data2 = ProcessPFC( mSource3, x_range_2, y_range_2 );


        QFont f = text_font;
        f.setPointSize( 6 );
        int w = (rect.height() - metrix.height())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines1;
        lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data1_e.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !data1_e2.empty() )
            lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data1_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        ff0x::NoAxisGraphBuilder::GraphDataLine lines2;
        lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !data2_e.empty() )
            lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2_e, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон", Qt::red ) ) );
        if ( !data2_e2.empty() )
            lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );


        QRect p1(rect.left(), rect.top(), w, h );
        QRect p2(rect.right() - w, rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
        QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());

        DrawRowCenter( p1t, text_font, Qt::black, "АЧХ. Амплитуда 3" );
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
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p1, builder.Draw( lines1, x_range, y_range, x_step, y_step, "Частота (Гц)", "Дб", true ) );
        }

        DrawRowCenter( p2t, text_font, Qt::black, "ФЧХ. Амплитуда 3" );
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
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p2, builder.Draw( lines2, x_range, y_range, x_step, y_step, "Частота (Гц)", "φ (гр.)", true ) );
        }
        painter.restore();
    }, 1, free_rect.width()/2 + metrix.height()  );
//    free_rect.setHeight( 0 );
    return res;
}

}//namespace servo

}//namespace test

