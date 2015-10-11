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
    bool Result; //Распределитель корректно переключается при всех допустимых значениях давления управления( от XX до XXX Бар )
};


}//namespace hydro

}//namespace test
