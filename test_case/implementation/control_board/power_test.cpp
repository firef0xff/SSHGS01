#include "power_test.h"
#include <QJsonObject>

namespace test
{

namespace control_board
{

PowerTest::PowerTest():
    test::control_board::Test( "Питание платы. Отсутствие ошибок", 15 ),
  Result(false)
{}

bool PowerTest::Run()
{
    return Result;
}

QJsonObject PowerTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("Result", Result );

    return obj;
}
bool PowerTest::Deserialize( QJsonObject const& obj )
{
    Result = obj.value("Result").toBool();
    return true;
}

}//namespace hydro

}//namespace test
