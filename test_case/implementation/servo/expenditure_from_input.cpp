#include "expenditure_from_input.h"
#include <QJsonObject>
#include <QJsonArray>

namespace test
{
namespace servo
{

ExpeditureFromInput::ExpeditureFromInput():
    test::servo::Test( "Зависимость расхода «к потребителю» от входного тока без нагрузки", 11 ),
    Gain(0),
    Hysteresis(0),
    Simmetry(0),
    Polar(0),
    Overlap(0)
{}

bool ExpeditureFromInput::Run()
{
    return true;
}
QJsonObject ExpeditureFromInput::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, Graph)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("Graph", a );
    obj.insert("Gain", Gain);
    obj.insert("Hysteresis", Hysteresis);
    obj.insert("Simmetry", Simmetry);
    obj.insert("Polar", Polar);
    obj.insert("Overlap", Overlap);

    return obj;
}
bool ExpeditureFromInput::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("Graph").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            Graph.insert( Graph.end(), d );
    }

    Gain = obj.value("Gain").toDouble();
    Hysteresis = obj.value("Hysteresis").toDouble();
    Simmetry = obj.value("Simmetry").toDouble();
    Polar = obj.value("Polar").toDouble();
    Overlap = obj.value("Overlap").toDouble();
    return true;
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
