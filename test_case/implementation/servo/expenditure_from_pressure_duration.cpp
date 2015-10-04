#include "expenditure_from_pressure_duration.h"
#include <QJsonObject>
#include <QJsonArray>

namespace test
{
namespace servo
{

ExpeditureFromPressureDuration::ExpeditureFromPressureDuration():
    test::servo::Test( "Зависимость расхода «к потребителю» от перепада давлений нагрузки", 12 )
{}

bool ExpeditureFromPressureDuration::Run()
{
    return true;
}
QJsonObject ExpeditureFromPressureDuration::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, BP5_3)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("BP5_3", a );

    QJsonArray b;
    foreach (Data const& d, BP3_V)
    {
        b.insert( b.end(), d.Serialise() );
    }
    obj.insert("BP3_V", a );

    return obj;
}
bool ExpeditureFromPressureDuration::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("BP5_3").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            BP5_3.insert( BP5_3.end(), d );
    }

    QJsonArray b = obj.value("BP3_V").toArray();
    foreach (QJsonValue const& v, b)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            BP3_V.insert( BP3_V.end(), d );
    }


    return true;
}

QJsonObject ExpeditureFromPressureDuration::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("Duration", Duration );
    obj.insert("Expenditure", Expenditure );

    return obj;
}
bool ExpeditureFromPressureDuration::Data::Deserialize( QJsonObject const& obj )
{
    Duration = obj.value("Duration").toDouble();
    Expenditure = obj.value("Expenditure").toDouble();
    return true;
}

}//namespace servo

}//namespace test
