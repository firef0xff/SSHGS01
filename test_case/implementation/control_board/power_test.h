#pragma once
#include "../test_base.h"

namespace test
{

namespace control_board
{

class PowerTest :public test::control_board::Test
{
public:
    PowerTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

    bool Success() const;

private:
    bool Result; ///если ошибка не выполнять следующий тест
};


}//namespace control_board

}//namespace test
