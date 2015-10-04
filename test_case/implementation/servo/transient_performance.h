#pragma once
#include "../test_base.h"
#include <QVector>

namespace test
{

namespace servo
{
class TransientPerformance :public test::servo::Test
{
public:
    TransientPerformance();
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

    QVector<Data> Graph;
};

}//namespace servo

}//namespace test
