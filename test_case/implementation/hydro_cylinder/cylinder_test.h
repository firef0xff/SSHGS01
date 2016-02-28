#pragma once
#include "../test_base.h"
#include <QVector>
#include <condition_variable>

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

    bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

    bool Success() const;
private:
    void Question();
    bool HermResult; //да если течь не обнаружена
    bool MaxPressureResult;
    bool WorkPressureResult;
    bool ExpenditureResult;
    bool MoveTimeResult;
    std::condition_variable mCondVar;

};

}//namespace hydro_cylinder

}//namespace test
