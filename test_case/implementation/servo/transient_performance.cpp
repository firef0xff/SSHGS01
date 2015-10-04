#include "transient_performance.h"
#include <QJsonObject>
#include <QJsonArray>

namespace test
{
namespace servo
{

TransientPerformance::TransientPerformance():
    test::servo::Test( "Переходные характеристики", 14 )
{}

bool TransientPerformance::Run()
{
    return true;
}
QJsonObject TransientPerformance::Serialise() const
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
bool TransientPerformance::Deserialize( QJsonObject const& obj )
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

QJsonObject TransientPerformance::Data::Serialise() const
{
    QJsonObject obj;
    obj.insert("x", x );
    obj.insert("y", y );

    return obj;
}
bool TransientPerformance::Data::Deserialize( QJsonObject const& obj )
{
    x = obj.value("x").toDouble();
    y = obj.value("y").toDouble();
    return true;
}

}//namespace servo

}//namespace test
