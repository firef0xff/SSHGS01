#include "max_expenditure.h"
#include <QJsonObject>

namespace test
{

namespace hydro
{

MaxExpenditureTest::MaxExpenditureTest():
    test::hydro::Test( "Проверка максимального расхода", 5 ),
  Result(false)
{}

bool MaxExpenditureTest::Run()
{
    return Result;
}

QJsonObject MaxExpenditureTest::Serialise() const
{
    QJsonObject obj;
    obj.insert("Result", Result );

    return obj;
}
bool MaxExpenditureTest::Deserialize( QJsonObject const& obj )
{
    Result = obj.value("Result").toBool();
    return true;
}

}//namespace hydro

}//namespace test
