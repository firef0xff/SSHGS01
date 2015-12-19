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

    bool Success() const;
    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;

protected:
    void UpdateData();
private:
    struct Data
    {
        double phase = 0;
        double ampl = 0;
        double frequency = 0;

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
    };

    QVector< Data > mData;
};

}//namespace servo

}//namespace test
