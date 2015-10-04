#pragma once
#include "../test_base.h"

namespace test
{

namespace hydro
{

class ActivationTime :public test::hydro::Test
{
public:
    ActivationTime();
    bool Run();
#warning //перед началом окно выбора динамики ?Oo

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );
private:
    bool ResultA; /// Время срабатывания (не)удовлетворяет
    bool ResultB; /// Время срабатывания (не)удовлетворяет
};


}//namespace hydro

}//namespace test
