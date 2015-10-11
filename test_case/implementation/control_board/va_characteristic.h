#pragma once
#include "../test_base.h"
#include <QVector>

namespace test
{

namespace control_board
{
class VACharacteristic :public test::control_board::Test
{
public:
    VACharacteristic();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;
private:
    struct Data
    {
        Data():
            x(0),
            y(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
        double x;
        double y;
    };

    QVector<Data> Graph;
    /// вероятно тоже график
    /// (хочу просто массива с контроллера для рисования )
};

}//namespace servo

}//namespace test
