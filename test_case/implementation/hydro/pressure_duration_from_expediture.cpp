#include "pressure_duration_from_expediture.h"
#include <QJsonObject>
#include <QJsonArray>

namespace test
{
namespace hydro
{

PressureDurationFromExpenditure::PressureDurationFromExpenditure():
    test::hydro::Test( "Проверка перепада давления и зависимость перепада давления от расхода", 4 )
{}

bool PressureDurationFromExpenditure::Run()
{

    return true;
}

QJsonObject PressureDurationFromExpenditure::Serialise() const
{
    QJsonObject obj;

    QJsonArray a;
    foreach (Data const& d, ChannelA)
    {
        a.insert( a.end(), d.Serialise() );
    }

    QJsonArray b;
    foreach (Data const& d, ChannelB)
    {
        a.insert( a.end(), d.Serialise() );
    }

    obj.insert("ChannelA", a );
    obj.insert("ChannelB", b );

    return obj;
}
bool PressureDurationFromExpenditure::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("ChannelA").toArray();
    foreach ( QJsonValue const& v, a )
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            ChannelA.push_back( d );
    }

    QJsonArray b = obj.value("ChannelB").toArray();
    foreach ( QJsonValue const& v, b )
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            ChannelB.push_back( d );
    }

    return true;
}

QJsonObject PressureDurationFromExpenditure::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("BP5", BP5 );
    obj.insert("BP3", BP3 );
    obj.insert("BP5_3", BP5_3 );
    obj.insert("Expenditure", Expenditure );

    return obj;
}
bool PressureDurationFromExpenditure::Data::Deserialize( QJsonObject const& obj )
{
    BP5 = obj.value("BP5").toDouble();
    BP3 = obj.value("BP3").toDouble();
    BP5_3 = obj.value("BP5_3").toDouble();
    Expenditure = obj.value("Expenditure").toDouble();
    return true;
}


}//namespace hydro

}//namespace test
