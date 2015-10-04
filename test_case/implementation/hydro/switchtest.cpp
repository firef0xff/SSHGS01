#include "switchtest.h"
#include <QJsonObject>

namespace test
{

namespace hydro
{

SwitchTest::SwitchTest():
    test::hydro::Test( "Проверка переключения запорно-регулирующего элемента пониженным напряжением", 6 ),
  Result(false)
{}

bool SwitchTest::Run()
{
    return Result;
}

QJsonObject SwitchTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("Result", Result );

    return obj;
}
bool SwitchTest::Deserialize( QJsonObject const& obj )
{
    Result = obj.value("Result").toBool();
    return true;
}

}//namespace hydro

}//namespace test
