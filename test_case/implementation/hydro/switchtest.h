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

    bool Draw(QPainter &painter, QRect &free_rect, QString  const& ) const;
    bool Success() const;

private:
    //Респределитель (не)переключается при допустимом пониженном напряжении управления

    bool ResultMinA;
    bool ResultMaxA;
    bool ResultMinB;
    bool ResultMaxB;

    bool ResultMinA_OFF;
    bool ResultMaxA_OFF;
    bool ResultMinB_OFF;
    bool ResultMaxB_OFF;
};


}//namespace hydro

}//namespace test
