#pragma once
#include "../test_base.h"
#include <condition_variable>

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
    std::condition_variable mCondVar;
    bool Result; ///если ошибка не выполнять следующий тест

    void Question();
};


}//namespace control_board

}//namespace test
