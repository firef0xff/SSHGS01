#pragma once
#include "../test_base.h"

namespace test
{

namespace hydro
{

class PressureRange :public test::hydro::Test
{
public:
    PressureRange();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect, QString  const& ) const;
    bool Success() const;
private:
    //Распределитель корректно переключается при всех допустимых значениях давления управления( от XX до XXX Бар )
    bool ResultMinMinA;
    bool ResultMinMaxA;
    bool ResultMinMinB;
    bool ResultMinMaxB;

    bool ResultMaxMinA;
    bool ResultMaxMaxA;
    bool ResultMaxMinB;
    bool ResultMaxMaxB;

    bool ResultMinMinA_OFF;
    bool ResultMinMaxA_OFF;
    bool ResultMinMinB_OFF;
    bool ResultMinMaxB_OFF;

    bool ResultMaxMinA_OFF;
    bool ResultMaxMaxA_OFF;
    bool ResultMaxMinB_OFF;
    bool ResultMaxMaxB_OFF;
};


}//namespace hydro

}//namespace test
