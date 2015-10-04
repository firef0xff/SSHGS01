#include "frequency_characteristics.h"
#include <QJsonObject>
#include <QJsonArray>

namespace test
{
namespace servo
{

FrequencyCharacteristics::FrequencyCharacteristics():
    test::servo::Test( "Частотные характеристики", 13 )
{}

bool FrequencyCharacteristics::Run()
{
    return true;
}
QJsonObject FrequencyCharacteristics::Serialise() const
{
    QJsonObject obj;
    QJsonArray a;
    foreach (Data const& d, AFC)
    {
        a.insert( a.end(), d.Serialise() );
    }
    obj.insert("AFC", a );

    QJsonArray b;
    foreach (Data const& d, FFC)
    {
        b.insert( b.end(), d.Serialise() );
    }
    obj.insert("FFC", a );

    return obj;
}
bool FrequencyCharacteristics::Deserialize( QJsonObject const& obj )
{
    QJsonArray a = obj.value("AFC").toArray();
    foreach (QJsonValue const& v, a)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            AFC.insert( AFC.end(), d );
    }

    QJsonArray b = obj.value("FFC").toArray();
    foreach (QJsonValue const& v, b)
    {
        Data d;
        if ( d.Deserialize( v.toObject() ) )
            FFC.insert( FFC.end(), d );
    }

    return true;
}

QJsonObject FrequencyCharacteristics::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("x", x );
    obj.insert("y", y );

    return obj;
}
bool FrequencyCharacteristics::Data::Deserialize( QJsonObject const& obj )
{
    y = obj.value("y").toDouble();
    x = obj.value("x").toDouble();
    return true;
}

}//namespace servo

}//namespace test
