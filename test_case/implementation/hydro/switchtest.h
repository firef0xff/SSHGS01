#pragma once
#include "../test_base.h"

namespace test
{

namespace hydro
{

class SwitchTest :public test::hydro::Test
{
public:
    SwitchTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );
private:
    bool Result;     //Респределитель (не)переключается при допустимом пониженном напряжении управления

};


}//namespace hydro

}//namespace test
