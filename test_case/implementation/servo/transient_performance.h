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
    struct Data
    {
        Data():
            time(0),
            position(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
        double time;
        double position;
    };
    typedef QVector<Data> Source;

    TransientPerformance();
    bool Run();
    bool Success() const;
    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;
protected:
    void UpdateData();
private:

    Source Graph1;
    Source Graph2;
    Source Graph3;
};

}//namespace servo

}//namespace test
