#pragma once
#include "../test_base.h"
#include <QVector>

namespace test
{

namespace servo
{
class FrequencyCharacteristics :public test::servo::Test
{
public:
    FrequencyCharacteristics();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );
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

    ///АЧХ, ФЧХ
    /// (хочу просто массива с контроллера для рисования )

    QVector<Data> AFC;
    QVector<Data> FFC;
};

}//namespace servo

}//namespace test
