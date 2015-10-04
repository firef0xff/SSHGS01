#include "activation_time.h"
#include <QJsonObject>

namespace test
{

namespace hydro
{

ActivationTime::ActivationTime():
    test::hydro::Test( "Время срабатывания", 8 ),
  ResultA(false),
  ResultB(false)
{}

bool ActivationTime::Run()
{
    return ResultA && ResultB;
}

QJsonObject ActivationTime::Serialise() const
{
    QJsonObject obj;
    obj.insert("ResultA", ResultA );
    obj.insert("ResultB", ResultB );

    return obj;
}
bool ActivationTime::Deserialize( QJsonObject const& obj )
{
    ResultA = obj.value("ResultA").toBool();
    ResultB = obj.value("ResultB").toBool();

    return true;
}

}//namespace hydro

}//namespace test

