#include "va_characteristic.h"
#include <QJsonObject>
#include <QJsonArray>

namespace test
{
namespace control_board
{

VACharacteristic::VACharacteristic():
    test::control_board::Test( "Построение зависимости выходного тока на катушку от входного опорного сигнала", 16 )
{}

bool VACharacteristic::Run()
{
    return true;
}
QJsonObject VACharacteristic::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, Graph)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("Graph", a );

    return obj;
}
bool VACharacteristic::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("Graph").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            Graph.insert( Graph.end(), d );
    }

    return true;
}

QJsonObject VACharacteristic::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("x", x );
    obj.insert("y", y );

    return obj;
}
bool VACharacteristic::Data::Deserialize( QJsonObject const& obj )
{
    x = obj.value("x").toDouble();
    y = obj.value("y").toDouble();
    return true;
}

}//namespace servo

}//namespace test
