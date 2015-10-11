#pragma once
#include "../test_base.h"
#include <QVector>

namespace test
{

namespace servo
{
class ExpeditureFromPressureDuration :public test::servo::Test
{
public:
    ExpeditureFromPressureDuration();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;

private:
    struct Data
    {
        Data():
            Duration(0),
            Expenditure(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
        double Duration;
        double Expenditure;
    };

    /// два массива данных BP5, BP3, BV ( для потебителя А и потребителя Б )
    /// строятся 2 графика зависимости значения перепада от расхода в канале
    /// (хочу просто массива с контроллера для рисования )

    QVector<Data> BP5_3;
    QVector<Data> BP3_V;
};

}//namespace servo

}//namespace test
