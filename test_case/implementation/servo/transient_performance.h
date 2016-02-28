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
    class GrapfData;
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

    void ResetDrawLine();
    bool Draw(QPainter &painter, QRect &free_rect, const QString &compare_width ) const;
protected:
    void UpdateData();
private:
    friend class GrapfData;
    Source Graph1;
    Source Graph2;
    Source Graph3;
    mutable GrapfData* mGrapfs;
};

}//namespace servo

}//namespace test
