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

    bool Draw(QPainter &painter, QRect &free_rect ) const;
private:
    //Респределитель (не)переключается при допустимом пониженном напряжении управления
    bool ResultMinMinA;
    bool ResultMinMaxA;
    bool ResultMinMinB;
    bool ResultMinMaxB;

    bool ResultMaxMinA;
    bool ResultMaxMaxA;
    bool ResultMaxMinB;
    bool ResultMaxMaxB;
};


}//namespace hydro

}//namespace test
