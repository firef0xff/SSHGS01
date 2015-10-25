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

    bool Draw(QPainter &painter, QRect &free_rect ) const;
private:
    //Распределитель корректно переключается при всех допустимых значениях давления управления( от XX до XXX Бар )
    bool ResultMinA;
    bool ResultMaxA;
    bool ResultMinB;
    bool ResultMaxB;
};


}//namespace hydro

}//namespace test
