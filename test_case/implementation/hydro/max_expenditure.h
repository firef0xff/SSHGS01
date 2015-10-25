#pragma once
#include "../test_base.h"

namespace test
{

namespace hydro
{

class MaxExpenditureTest :public test::hydro::Test
{
public:
    MaxExpenditureTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;
private:
    bool ResultA; //Максимальный расход (не)удовлетворяет установленным критериям
    bool ResultB;
};


}//namespace hydro

}//namespace test
