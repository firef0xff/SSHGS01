#pragma once
#include "../test_base.h"
#include <QVector>

namespace test
{

namespace hydro_cylinder
{

class FunctionalTest :public test::hydro_cylinder::Test
{
public:
    FunctionalTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );
private:
    void Question();
    bool Result; /// в отчет список параметров
    /// и диалоговое окно пригоден не пригоден для оператора

};

}//namespace hydro_cylinder

}//namespace test
