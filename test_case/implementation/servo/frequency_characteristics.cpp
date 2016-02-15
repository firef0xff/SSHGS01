#include "frequency_characteristics.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include "test_case/test_params.h"
#include "../test_params_servo.h"
#include <thread>
#define DEBUG

#ifdef DEBUG
#include <QJsonDocument>
#include <QFile>
#endif
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

ff0x::NoAxisGraphBuilder::LinePoints Approximate( ff0x::NoAxisGraphBuilder::LinePoints const& r )
{
    if ( r.size() < 3 )
        return r;

    ff0x::NoAxisGraphBuilder::LinePoints res;
    QPointF las_inserted;
    for ( int i = 0; i < r.size(); ++i )
    {
        if ( i == 0 )
        {
            las_inserted = r[i];
            res.push_back( r[i] );
        }
        else if  ( i < r.size() - 1 )
        {
            double a = atan( ( r[ i + 1 ].y() - las_inserted.y() ) / ( r[ i + 1 ].x() - las_inserted.x() ) );
            double b = atan( ( r[ i ].y() - las_inserted.y() ) / ( r[ i ].x() - las_inserted.x() ) );
            double g = ( b - a )/2 + a;
            double y = tan( g ) * ( r[ i ].x() - las_inserted.x() ) + las_inserted.y();
            res.push_back( QPointF( r[i].x(), y ) );
        }
        else
        {
            las_inserted = r[i];
            res.push_back( r[i] );
        }
    }
    return std::move( res );
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

    template < class Strategy >
    GrapfInfo( QVector<QPointF> const& data, Strategy s )
    {
        s.Process( data, Info );
    }

    Data const& GetInfo() const
    {
        return Info;
    }
private:
    Data Info;
};

class FollowAnaliser
{
public:
    void Process( QVector<QPointF> const& data, GrapfInfo::Data &Info )
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
                    Info.push_back(GrapfInfo::PointInfo( curr_p, GrapfInfo::PointInfo::Min ));
                else if ( way < 0 )
                    Info.push_back(GrapfInfo::PointInfo( curr_p, GrapfInfo::PointInfo::Max ));
                old_way = way;
            }
        }
        if ( old_way )
        {
            if ( old_way < 0 )
                Info.push_back(GrapfInfo::PointInfo( curr_p, GrapfInfo::PointInfo::Min ));
            else if ( old_way > 0 )
                Info.push_back(GrapfInfo::PointInfo( curr_p, GrapfInfo::PointInfo::Max ));
        }
    }
};
class SinusAnaliser
{
public:
    void Process( QVector<QPointF> const& data, GrapfInfo::Data &Info )
    {
        int old_way = 0;
        QPointF curr_p;

        int i_max = 0, i_min = 0;
        for ( int i = 0; i < data.size(); ++i )
        {
            curr_p = data[i];
            if ( curr_p.y() < data[i_min].y() )
                i_min = i;
            if ( curr_p.y() > data[i_max].y() )
                i_max = i;
        }

        double zero_lvl = data[i_min].y() + (data[i_max].y() - data[i_min].y())/2;

        QPointF max_p(0,zero_lvl);
        QPointF min_p(0,zero_lvl);
        for ( int i = 0; i < data.size(); ++i )
        {
            curr_p = data[i];
            if ( !i )
                Info.push_back(GrapfInfo::PointInfo( curr_p, GrapfInfo::PointInfo::Unset ));
            int way = curr_p.y() > zero_lvl ? 1 : curr_p.y() < zero_lvl ? -1 : old_way;
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
                    Info.push_back(GrapfInfo::PointInfo( min_p, GrapfInfo::PointInfo::Min ));
                    min_p = QPointF( 0, zero_lvl );
                }
                else if ( way < 0 )
                {
                    Info.push_back(GrapfInfo::PointInfo( max_p, GrapfInfo::PointInfo::Max ));
                    max_p = QPointF( 0, zero_lvl );
                }
            }
            old_way = way;
        }
        if ( old_way )
        {
            if ( old_way < 0 )
                Info.push_back(GrapfInfo::PointInfo( min_p, GrapfInfo::PointInfo::Min ));
            else if ( old_way > 0 )
                Info.push_back(GrapfInfo::PointInfo( max_p, GrapfInfo::PointInfo::Max ));
        }
        Info.push_back(GrapfInfo::PointInfo( curr_p, GrapfInfo::PointInfo::Unset ));
    }
};

class StepAnaliser
{
public:
    void Process( QVector<QPointF> const& data, GrapfInfo::Data &Info )
    {
        int old_way = 0;
        QPointF curr_p;
        GrapfInfo::Data inf;
        int i_max = 0, i_min = 0;
        for ( int i = 0; i < data.size(); ++i )
        {
            curr_p = data[i];
            if ( curr_p.y() < data[i_min].y() )
                i_min = i;
            if ( curr_p.y() > data[i_max].y() )
                i_max = i;
        }
        double range = data[i_max].y() - data[i_min].y();
//        double zero_lvl = data[i_min].y() + (data[i_max].y() - data[i_min].y())/2;

        QPointF start_p;
        QPointF stop_p;
        for ( int i = 0; i < data.size(); ++i )
        {
            curr_p = data[i];
            if ( !i )
            {
                start_p = curr_p;
                continue;
            }

            double dist_top = fabs( data[i].y() - data[ i_max ].y() );
            double dist_bottom = fabs(data[i].y() - data[ i_min ].y() );
            int way = 0;
            if ( dist_top <= range / 100 * 15 ||
                 dist_bottom <= range / 100 * 15 )
            {
                //зона не чувствительности
                way = 1;
            }
            else
            {
                //зона чувствительности
                way = -1;
            }

            if ( way != old_way && old_way )
            {
                if ( way == 1 )
                {
                    inf.push_back(GrapfInfo::PointInfo( start_p, GrapfInfo::PointInfo::Min ));
                    inf.push_back(GrapfInfo::PointInfo( stop_p, GrapfInfo::PointInfo::Max ));
                }
            }

            if ( way == 1 )
                start_p = curr_p;
            else if ( way == -1 )
                stop_p = curr_p;

            old_way = way;
        }
        if ( old_way < 0 )
        {
            inf.push_back(GrapfInfo::PointInfo( start_p, GrapfInfo::PointInfo::Min ));
            inf.push_back(GrapfInfo::PointInfo( stop_p, GrapfInfo::PointInfo::Max ));
        }


        //найти самый большой отрезок по dy
        int f_max = 0;
        for ( int i = 0; i < inf.size(); ++i )
        {
            if ( !i )
                continue;

            if ( inf[ i ].type != GrapfInfo::PointInfo::Max ||
                 inf[ i - 1 ].type != GrapfInfo::PointInfo::Min)
                continue;

            if ( inf[ i ].point.y() < inf[ i - 1 ].point.y() )
                continue;

            if ( !f_max )
                f_max = i;

            GrapfInfo::PointInfo const& point = inf[i];
            GrapfInfo::PointInfo const& prev = inf[i-1];

            if ( fabs(point.point.y() - prev.point.y()) >
                 fabs(inf[f_max].point.y() - inf[f_max-1].point.y()))
                f_max = i;
        }
        if (!inf.empty() && f_max )
        {
            Info.push_back( inf[f_max-1] );
            Info.push_back( inf[f_max] );
        }
    }
};

class SinusAnaliser2
{
    enum ZoneType
    {
        undefined,
        plus,
        minus,
        zero
    };
    struct Zone
    {
        int start = -1;
        int stop = -1;
        ZoneType type = undefined;
    };
public:
    void Process( QVector<QPointF> const& data, GrapfInfo::Data &Info )
    {
        // опрередить амплитуду
        // определить зоны + и зоны -
        QVector< Zone > zones;
        Zone current_zone;
        int max_y = 0;
        int min_y = 0;

        for ( int i = 0; i < data.size(); ++i )
        {
            QPointF const& p = data[i];

            if ( p.y() > data[max_y].y() )
                max_y = i;
            if ( p.y() < data[min_y].y() )
                min_y = i;

            // определим находимся ли мы еще в прошлой зоне
            if ( ( p.y() < 0 && current_zone.type != minus ) ||
                 ( p.y() > 0 && current_zone.type != plus ) ||
                 ( p.y() == 0 && current_zone.type != zero ) )
            {
                if ( current_zone.type != undefined )
                {
                    zones.push_back( current_zone );
                    current_zone = Zone();
                }
            }

            if ( current_zone.start == -1 )
            {
                current_zone.start = i;
                if ( p.y() < 0 )
                    current_zone.type = minus;
                else if ( p.y() > 0 )
                    current_zone.type = plus;
                else
                    current_zone.type = zero;
            }
            current_zone.stop = i;
        }
        if ( current_zone.type != undefined )
            zones.push_back( current_zone );

        // уточнить околонулевые зоны
        // околонулевыими зонами будут считаться зоны максимум которых меньше 50 % от масимума амплидуты
        for ( int i = 0; i < zones.size(); ++i )
        {
            Zone &z = zones[i];
            if ( z.type != zero )
            {
                int max = z.start;
                int min = z.start;
                for ( int i = z.start; i <= z.stop; ++i )
                {
                    QPointF const& p = data[i];

                    if ( p.y() > data[max].y() )
                        max = i;
                    if ( p.y() < data[min].y() )
                        min = i;
                }

                if ( z.type == plus && data[max].y() < 0.5 * data[max_y].y() )
                    z.type = zero;

                if ( z.type == minus && data[min].y() > 0.5 * data[min_y].y() )
                    z.type = zero;
            }
        }

        // объеденить околонулевые зоны
        int splitted = 0;
        do
        {
            splitted = 0;
            QVector< Zone > split_zones;
            for ( int i = 0; i < zones.size(); ++i )
            {
                if (!i)
                    continue;

                Zone &pz = zones[i-1];
                Zone &z = zones[i];
                if ( pz.type == zero && z.type == zero )
                {
                    ++splitted;
                    pz.stop = z.stop;
                    z.type = undefined;
                }
            }

            for ( int i = 0; i < zones.size(); ++i )
            {
                Zone &z = zones[i];
                if ( z.type != undefined )
                {
                    split_zones.push_back( z );
                }
            }
            zones = split_zones;
        } while( splitted );
/*        // околонулевые зоны дробятся на две части на + и на -
        do
        {
            splitted = 0;
            QVector< Zone > split_zones;
            for ( int i = 0; i < zones.size(); ++i )
            {
                if (!i)
                    continue;

                Zone &pz = zones[i-1];
                Zone &z = zones[i];
                if ( pz.type == zero && z.type != zero )
                {
                    ++splitted;
                    z.start = pz.start;
                    pz.type = undefined;
                }
                if ( pz.type != zero && pz.type != undefined && z.type == zero )
                {
                    double count = 0;
                    if ( z.stop - z.start > 0 )
                        count = ceil( double(z.stop - z.start)/2.0 );
                    if ( count )
                    {
                        ++splitted;
                        pz.stop += count;
                        z.start += count;
                    }
                }
            }

            for ( int i = 0; i < zones.size(); ++i )
            {
                Zone &z = zones[i];
                if ( z.type != undefined )
                {
                    split_zones.push_back( z );
                }
            }
            zones = split_zones;
        } while( splitted );
*/
        //объеденить рядом стоящие зоны
        do
        {
            splitted = 0;
            QVector< Zone > split_zones;
            for ( int i = 0; i < zones.size(); ++i )
            {
                if (!i)
                    continue;

                Zone &pz = zones[i-1];
                Zone &z = zones[i];
                if ( pz.type == plus && z.type == plus )
                {
                    ++splitted;
                    pz.stop = z.stop;
                    z.type = undefined;
                }
            }

            for ( int i = 0; i < zones.size(); ++i )
            {
                Zone &z = zones[i];
                if ( z.type != undefined )
                {
                    split_zones.push_back( z );
                }
            }
            zones = split_zones;
        } while( splitted );

        do
        {
            splitted = 0;
            QVector< Zone > split_zones;
            for ( int i = 0; i < zones.size(); ++i )
            {
                if (!i)
                    continue;

                Zone &pz = zones[i-1];
                Zone &z = zones[i];
                if ( pz.type == minus && z.type == minus )
                {
                    pz.stop = z.stop;
                    z.type = undefined;
                    ++splitted;
                }
            }

            for ( int i = 0; i < zones.size(); ++i )
            {
                Zone &z = zones[i];
                if ( z.type != undefined )
                {
                    split_zones.push_back( z );
                }
            }
            zones = split_zones;
        } while( splitted );

        // поставить точку равную +- половине амплитуды на середине зон + -
        for ( int i = 0; i < zones.size(); ++i )
        {
            Zone &z = zones[i];

            int j = z.start + ceil( double( z.stop - z.start )/2.0 );

            if ( z.type == plus )
            {
                QPointF p( data[j].x(), data[max_y].y() );
                Info.push_back(GrapfInfo::PointInfo( p, GrapfInfo::PointInfo::Max ));
            }
            if ( z.type == minus )
            {
                QPointF p( data[j].x(), data[min_y].y() );
                Info.push_back(GrapfInfo::PointInfo( p, GrapfInfo::PointInfo::Min ));
            }
            if ( z.type == zero )
            {
                QPointF p( data[j].x(), 0 );
                Info.push_back(GrapfInfo::PointInfo( p, GrapfInfo::PointInfo::Unset ));
            }
        }
    }
};

typedef SinusAnaliser2 DefaultAnaliser;

#ifdef DEBUG
QJsonArray AFC_DEBUG;
QJsonArray PFC_DEBUG;
#endif
double CalckAmpl( FrequencyCharacteristics::DataSet const& data )
{
    if (data.empty())
        return 0.0;

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
double CalckAmpl3( FrequencyCharacteristics::DataSet const& data, double frequency = 0 )
{
#ifdef DEBUG
    QJsonObject obj;
    obj.insert( "частота", frequency );
#endif
    if (data.empty())
        return 0.0;

    QVector< QPointF > pos;
    for ( size_t i = 0; i < data.size(); ++i )
    {
        pos.push_back( QPointF( i, data[i].position ) );
    }

    GrapfInfo info_pos( pos, StepAnaliser() );
    GrapfInfo::Data const& inf = info_pos.GetInfo();

    double sum_speed = 0;
    int count = 0;
    //найти самый большой отрезок по dy
    int i_max = 0;
    for ( int i = 0; i < inf.size(); ++i )
    {
        if ( !i )
            continue;

        if ( inf[ i ].type != GrapfInfo::PointInfo::Max ||
             inf[ i - 1 ].type != GrapfInfo::PointInfo::Min)
            continue;
        if ( !i_max )
            i_max = i;

        GrapfInfo::PointInfo const& point = inf[i];
        GrapfInfo::PointInfo const& prev = inf[i-1];

        if ( fabs(point.point.y() - prev.point.y()) >
             fabs(inf[i_max].point.y() - inf[i_max-1].point.y()))
            i_max = i;
    }
    if ( !inf.empty() )
    {
        sum_speed += fabs(inf[i_max].point.y() - inf[i_max-1].point.y()) / ( inf[i_max].point.x() - inf[i_max-1].point.x() );
#ifdef DEBUG
        obj.insert( "x1", inf[i_max-1].point.x() );
        obj.insert( "y1", inf[i_max-1].point.y() );
        obj.insert( "x2", inf[i_max].point.x() );
        obj.insert( "y2", inf[i_max].point.y() );
        obj.insert( "sum_speed", sum_speed );
        AFC_DEBUG.push_back(obj);
#endif
    }
    sum_speed = sum_speed / ( count ? count : 1 );
    return sum_speed;
}

ff0x::NoAxisGraphBuilder::LinePoints ProcessAFC( FrequencyCharacteristics::Source const& src, QPointF& x_range, QPointF& y_range )
{
#ifdef DEBUG
    AFC_DEBUG = QJsonArray();
#endif

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
        if ( it->second.empty() )
            continue;

        double ampl = CalckAmpl3( it->second, it->first ) *K / time_period;
        if ( it == src.begin() )
            min_ampl = ampl;

//        point.setY(ampl);

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
#else
    QFile f( "AFC_DEBUG" + QString::number( reinterpret_cast<long long>( &src ) ) );
    f.open(QIODevice::WriteOnly);
    QJsonDocument d;
    d.setArray( AFC_DEBUG );
    f.write( d.toJson() );
    f.close();
#endif
        result.push_back( point );
    }
    return Approximate( result );
}

double CalckFi( FrequencyCharacteristics::DataSet const& data, double frequency )
{
    if (data.empty())
        return 0;
#ifdef DEBUG
    QJsonObject obj;
    obj.insert( "частота", frequency );
#endif
    QVector< QPointF > speed;
    QVector< QPointF > signal;
    for ( size_t i = 0; i < data.size(); ++i )
    {
        signal.push_back( QPointF( i, data[i].signal ) );
        speed.push_back( QPointF( i, data[i].position) );
    }

    GrapfInfo info_speed( speed, StepAnaliser() );
    GrapfInfo info_signal( signal, DefaultAnaliser() );

//        ищем координату первого максимума сигнала
    GrapfInfo::Data const& inf = info_signal.GetInfo();
    double signal_max_time = 0;
    for ( int i = 0; i < inf.size(); ++i )
    {
        GrapfInfo::PointInfo const& point = inf[i];
        if ( point.type == GrapfInfo::PointInfo::Max )
        {
            signal_max_time = point.point.x();
            break;
        }
    }

//вычисляем позицию максимума скорости
    QPointF min;
    QPointF max;
    GrapfInfo::Data const& inf_sg = info_speed.GetInfo();
    for ( int i = 0; i < inf_sg.size(); ++i )
    {
        GrapfInfo::PointInfo const& point = inf_sg[i];
        if ( point.type == GrapfInfo::PointInfo::Max  )
        {
            max = point.point;
        }
        if ( point.type == GrapfInfo::PointInfo::Min  )
        {
            min = point.point;
        }
    }
    double speed_max_time = min.x() + (max.x() - min.x())/2.0;
    double Tsm = speed_max_time - signal_max_time;
    double T = 1/frequency;
    double fi = Tsm/(1000*T)*360;
#ifdef DEBUG
    obj.insert( "x1", max.x() );
    obj.insert( "x2", min.x() );
    obj.insert( "speed_max_time", speed_max_time );
    obj.insert( "signal_max_time", signal_max_time );
    obj.insert( "Tsm", Tsm );
    obj.insert( "T", T );
    obj.insert( "fi", fi );
    PFC_DEBUG.push_back(obj);
#endif
    return fi;
}

ff0x::NoAxisGraphBuilder::LinePoints ProcessPFC( FrequencyCharacteristics::Source const& src, QPointF& x_range, QPointF& y_range )
{
#ifdef DEBUG
    PFC_DEBUG = QJsonArray();
#endif

    ff0x::NoAxisGraphBuilder::LinePoints result;
    for ( auto it = src.begin(), end = src.end(); it != end; ++it  )
    {
        QPointF point;
        point.setX( it->first );
        if ( it->second.empty() )
            continue;

        auto fi = CalckFi( it->second, it->first );
        if ( fi <= 0.0 )
            continue;
        point.setY( fi );

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
#else
    QFile f( "PFC_DEBUG" + QString::number( reinterpret_cast<long long>( &src ) ) );
    f.open(QIODevice::WriteOnly);
    QJsonDocument d;
    d.setArray( PFC_DEBUG );
    f.write( d.toJson() );
    f.close();
#endif

        result.push_back( point );
    }
    return Approximate( result );
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
                double y = 0;
                if ( !j )
                    y = lnk[j + 1].position - lnk[j].position;
                else
                    y = lnk[j].position - lnk[j-1].position;
                if ( !y )
                    continue;
                point.setY( y );
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
                point.setY( lnk[j].position );
                data.push_back( point );
            }
            if (data.isEmpty())
                continue;

            GrapfInfo inf( data, StepAnaliser() );
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

class FrequencyCharacteristics::GrapfData
{
public:
    GrapfData( FrequencyCharacteristics const* test, int source, QString compare_width )
    {
        FrequencyCharacteristics::Source const*src = nullptr;
        QString src_str;

        switch (source)
        {
        case 1:
            src = &test->mSource1;
            src_str = "Source1";
            break;
        case 2:
            src = &test->mSource2;
            src_str = "Source2";
            break;
        case 3:
            src = &test->mSource3;
            src_str = "Source3";
            break;
        default:
            return;
        }

        QPointF x_range_e1;
        QPointF y_range_e1;

        QPointF x_range_e2;
        QPointF y_range_e2;
        //поиск данных теста
        bool use_etalone = false;
        foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
        {
            auto obj = val.toObject();
            if ( obj.value("id").toInt() == test->mId )
            {
                QJsonArray a = obj.value("data").toObject().value(src_str).toArray();
                data1_e2 = ProcessAFC( FromJson( a ), x_range_e1, y_range_e1 );
                data2_e2 = ProcessPFC( FromJson( a ), x_range_e2, y_range_e2 );
                use_etalone = true;
            }
        }

        data1 = ProcessAFC( *src, x_range_1, y_range_1 );
        data2 = ProcessPFC( *src, x_range_2, y_range_2 );
        x_range_1 = ff0x::MergeRanges( x_range_1, x_range_e1, use_etalone );
        y_range_1 = ff0x::MergeRanges( y_range_1, y_range_e1, use_etalone );

        x_range_2 = ff0x::MergeRanges( x_range_2, x_range_e2, use_etalone );
        y_range_2 = ff0x::MergeRanges( y_range_2, y_range_e2, use_etalone );
    }

    ff0x::GraphBuilder::LinePoints data1;
    ff0x::GraphBuilder::LinePoints data1_e2;

    ff0x::GraphBuilder::LinePoints data2;
    ff0x::GraphBuilder::LinePoints data2_e2;

    QPointF x_range_1;
    QPointF y_range_1;

    QPointF x_range_2;
    QPointF y_range_2;
};

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


    if ( ReelControl() )
    {
        Ampl1 = mControlReelBits.ampl1;
        Ampl2 = mControlReelBits.ampl2;
        Ampl3 = mControlReelBits.ampl3;
    }
    else
    {
        Ampl1 = mControlBoardBits.ampl1;
        Ampl2 = mControlBoardBits.ampl2;
        Ampl3 = mControlBoardBits.ampl3;
    }

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

    if ( src->operator []( *frequency ).size() == m1525Counts.OP15_25_count )
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
        src->operator []( *frequency ) = data;
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

    obj.insert( "Ampl1", Ampl1 );
    obj.insert( "Ampl2", Ampl2 );
    obj.insert( "Ampl3", Ampl3 );

    return obj;
}
bool FrequencyCharacteristics::Deserialize( QJsonObject const& obj )
{
    mSource1 = FromJson( obj.value("Source1").toArray() );
    mSource2 = FromJson( obj.value("Source2").toArray() );
    mSource3 = FromJson( obj.value("Source3").toArray() );

    Ampl1 = obj.value("Ampl1").toDouble();
    Ampl2 = obj.value("Ampl2").toDouble();
    Ampl3 = obj.value("Ampl3").toDouble();

    Test::Deserialize( obj );
    return true;
}
void FrequencyCharacteristics::ResetDrawLine()
{
    Test::ResetDrawLine();
    if ( mGrapfs1 )
    {
        delete mGrapfs1;
        mGrapfs1 = nullptr;
    }
    if ( mGrapfs2 )
    {
        delete mGrapfs2;
        mGrapfs2 = nullptr;
    }
    if ( mGrapfs3 )
    {
        delete mGrapfs3;
        mGrapfs3 = nullptr;
    }
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

    test::servo::Parameters old;
    QJsonObject f = test::ReadFromFile( compare_width ).value("Params").toObject();
    old.Deserialize( f );


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


    QString str_e_pn =          !compare_width.isEmpty() ? " (" +test::ToString(old.PressureNominal()) + ")" : QString();
    QString str_e_dexp =        !compare_width.isEmpty() ? " (" +test::ToString(old.DefaultExpenditure()) + ")" : QString();

    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, &params, str_e_pn ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, test::ToString( params->PressureNominal() ) + str_e_pn  );
    }, 2 );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &DrawRowLeft, &FillToSize, &text_font, &params, str_e_dexp ]( QRect const& rect )
    {
        DrawRowLeft( rect, text_font, Qt::black, FillToSize("Расход при проведении испытаний, л/мин"), Qt::red, test::ToString( params->DefaultExpenditure() ) + str_e_dexp );
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
    if (!mGrapfs1)
        mGrapfs1 = new GrapfData( this, 1, compare_width );
    if (!mGrapfs2)
        mGrapfs2 = new GrapfData( this, 2, compare_width );
    if (!mGrapfs3)
        mGrapfs3 = new GrapfData( this, 3, compare_width );

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
                f.setPointSize( 12 );
                int w = (rect.height() - metrix.height())*0.98;
                int h = (rect.height() - metrix.height())*0.98;

                ff0x::NoAxisGraphBuilder builder ( w, h, f );
                ff0x::NoAxisGraphBuilder::GraphDataLine lines1;
                ff0x::NoAxisGraphBuilder::GraphDataLine lines2;
                lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data, ff0x::NoAxisGraphBuilder::LabelInfo( "", Qt::blue ) ) );
                lines1.push_back( ff0x::NoAxisGraphBuilder::Line(data3, ff0x::NoAxisGraphBuilder::LabelInfo( "", Qt::red ) ) );
                lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data2, ff0x::NoAxisGraphBuilder::LabelInfo( "", Qt::red ) ) );
//                lines2.push_back( ff0x::NoAxisGraphBuilder::Line(data3, ff0x::NoAxisGraphBuilder::LabelInfo( "", Qt::blue ) ) );

                QRect p1(rect.left(), rect.top(), w, h );
                QRect p2(rect.right() - w, rect.top(), w, h );
                QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
                QRect p2t(p2.left(), p2.bottom(), p2.width(), metrix.height());

                DrawRowCenter( p1t, text_font, Qt::black, "результат: " + QString::number( i + 1  ) );
                {
                    QPointF x_range;
                    QPointF y_range;
                    double x_step = 0;
                    double y_step = 0;

    //                ff0x::DataLength( x_range_1, x_range_2, true, x_range, x_step );
    //                ff0x::DataLength( y_range_1, y_range_2, true, y_range, y_step );

                    ff0x::DataLength( ff0x::MergeRanges( x_range_1, x_range_3, true ), x_range, x_step );
                    ff0x::DataLength( ff0x::MergeRanges( y_range_1, y_range_3, true ), y_range, y_step );
//                    ff0x::DataLength( x_range_1, x_range, x_step );
//                    ff0x::DataLength( y_range_1, y_range, y_step );

                    if ( y_range.x() - y_range.y() == 0 )
                    {
                        y_range.setX( 1 );
                        y_range.setY( -1 );
                    }
                    painter.drawPixmap( p1, builder.Draw( lines1, x_range, y_range, x_step, y_step, "x", "y", true ) );
                }

                auto it2 = s.end();
                int k = 0;
                for ( auto it = s.begin(), end = s.end(); it != end; ++it  )
                {
                    if ( i == k )
                    {
                        it2 = it;
                    }
                    ++k;

                }
                if ( it2 != s.end() )
                    DrawRowCenter( p2t, text_font, Qt::black, "результат: " + QString::number( it2->first ) );
                {
                    QPointF x_range;
                    QPointF y_range;
                    double x_step = 0;
                    double y_step = 0;


//                    ff0x::DataLength( ff0x::MergeRanges( x_range_2, x_range_3, true ), x_range, x_step );
//                    ff0x::DataLength( ff0x::MergeRanges( y_range_2, y_range_3, true ), y_range, y_step );

                    ff0x::DataLength( x_range_2, x_range, x_step );
                    ff0x::DataLength( y_range_2, y_range, y_step );

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
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width, &params ]( QRect const& rect )
    {
        painter.save();

        QFont f = text_font;
        f.setPointSize( 12 );
        int w = (rect.width())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs1->data1, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !mGrapfs1->data1_e2.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs1->data1_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());

        QString amp = test::ToString( Ampl1 );
        DrawRowCenter( p1t, text_font, Qt::black, "АЧХ. Амплитуда " + amp + "%" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;            

            ff0x::DataLength( mGrapfs1->x_range_1,x_range, x_step );
            ff0x::DataLength( mGrapfs1->y_range_1,y_range, y_step );
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p1, builder.Draw( lines, x_range, y_range, x_step, y_step, "Частота (Гц)", "Дб", true ) );
        }
        painter.restore();
    }, 1, 480  );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width, &params ]( QRect const& rect )
    {
        painter.save();

        QFont f = text_font;
        f.setPointSize( 12 );
        int w = (rect.width())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs1->data2, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !mGrapfs1->data2_e2.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs1->data2_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());

        QString amp = test::ToString( Ampl1 );
        DrawRowCenter( p1t, text_font, Qt::black, "ФЧХ. Амплитуда " + amp + "%"  );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;
            ff0x::DataLength( mGrapfs1->x_range_2,x_range, x_step );
            ff0x::DataLength( mGrapfs1->y_range_2,y_range, y_step );
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p1, builder.Draw( lines, x_range, y_range, x_step, y_step, "Частота (Гц)", "φ (гр.)", true ) );
        }
        painter.restore();
    }, 1, 480  );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width, &params ]( QRect const& rect )
    {
        painter.save();

        QFont f = text_font;
        f.setPointSize( 12 );
        int w = (rect.width())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs2->data1, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !mGrapfs2->data1_e2.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs2->data1_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());

        QString amp = test::ToString( Ampl2 );
        DrawRowCenter( p1t, text_font, Qt::black, "АЧХ. Амплитуда " + amp + "%" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;

            ff0x::DataLength( mGrapfs2->x_range_1,x_range, x_step );
            ff0x::DataLength( mGrapfs2->y_range_1,y_range, y_step );
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p1, builder.Draw( lines, x_range, y_range, x_step, y_step, "Частота (Гц)", "Дб", true ) );
        }
        painter.restore();
    }, 1, 480  );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width, &params ]( QRect const& rect )
    {
        painter.save();

        QFont f = text_font;
        f.setPointSize( 12 );
        int w = (rect.width())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs2->data2, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !mGrapfs2->data2_e2.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs2->data2_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());

        QString amp = test::ToString( Ampl2 );
        DrawRowCenter( p1t, text_font, Qt::black, "ФЧХ. Амплитуда " + amp + "%"  );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;
            ff0x::DataLength( mGrapfs2->x_range_2,x_range, x_step );
            ff0x::DataLength( mGrapfs2->y_range_2,y_range, y_step );
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p1, builder.Draw( lines, x_range, y_range, x_step, y_step, "Частота (Гц)", "φ (гр.)", true ) );
        }
        painter.restore();
    }, 1, 480  );


    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width, &params ]( QRect const& rect )
    {
        painter.save();

        QFont f = text_font;
        f.setPointSize( 12 );
        int w = (rect.width())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs3->data1, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !mGrapfs3->data1_e2.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs3->data1_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());

        QString amp = test::ToString( Ampl3 );
        DrawRowCenter( p1t, text_font, Qt::black, "АЧХ. Амплитуда " + amp + "%" );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;

            ff0x::DataLength( mGrapfs3->x_range_1,x_range, x_step );
            ff0x::DataLength( mGrapfs3->y_range_1,y_range, y_step );
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p1, builder.Draw( lines, x_range, y_range, x_step, y_step, "Частота (Гц)", "Дб", true ) );
        }
        painter.restore();
    }, 1, 480  );
    res = DrawLine( num, free_rect, text_font,
    [ this, &painter, &text_font, &DrawRowCenter, &metrix, &compare_width, &params ]( QRect const& rect )
    {
        painter.save();

        QFont f = text_font;
        f.setPointSize( 12 );
        int w = (rect.width())*0.98;
        int h = (rect.height() - metrix.height())*0.98;

        ff0x::NoAxisGraphBuilder builder ( w, h, f );
        ff0x::NoAxisGraphBuilder::GraphDataLine lines;
        lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs3->data2, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат", Qt::blue ) ) );
        if ( !mGrapfs3->data2_e2.empty() )
            lines.push_back( ff0x::NoAxisGraphBuilder::Line(mGrapfs3->data2_e2, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат", Qt::gray ) ) );

        QRect p1(rect.left(), rect.top(), w, h );
        QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());

        QString amp = test::ToString( Ampl3 );
        DrawRowCenter( p1t, text_font, Qt::black, "ФЧХ. Амплитуда " + amp + "%"  );
        {
            QPointF x_range;
            QPointF y_range;
            double x_step = 0;
            double y_step = 0;
            ff0x::DataLength( mGrapfs3->x_range_2,x_range, x_step );
            ff0x::DataLength( mGrapfs3->y_range_2,y_range, y_step );
            if ( y_range.x() - y_range.y() == 0 )
            {
                y_range.setX( 1 );
                y_range.setY( -1 );
            }
            painter.drawPixmap( p1, builder.Draw( lines, x_range, y_range, x_step, y_step, "Частота (Гц)", "φ (гр.)", true ) );
        }
        painter.restore();
    }, 1, 480  );

    free_rect.setHeight( 0 );
    return res;
}

}//namespace servo

}//namespace test

