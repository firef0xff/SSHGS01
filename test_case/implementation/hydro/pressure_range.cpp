#include "pressure_range.h"
#include <QJsonObject>

namespace test
{

namespace hydro
{

PressureRange::PressureRange():
    test::hydro::Test( "Проверка диапазона давления управления\n(для направляющей гидроаппаратуры с электрогидравлическим управлением)", 7 ),
  Result(false)
{}

bool PressureRange::Run()
{
    return Result;
}

QJsonObject PressureRange::Serialise() const
{
    QJsonObject obj;
    obj.insert("Result", Result );

    return obj;
}
bool PressureRange::Deserialize( QJsonObject const& obj )
{
    Result = obj.value("Result").toBool();
    return true;
}

}//namespace hydro

}//namespace test
