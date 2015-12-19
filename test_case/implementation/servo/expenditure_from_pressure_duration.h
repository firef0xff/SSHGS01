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
    bool Success() const;
    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;

    void ResetDrawLine();
private:
    struct Data
    {
        struct Channel
        {
            Channel():
                BP5(0),
                BP3(0)
            {}
            double BP5;
            double BP3;
            double BP5_3 () const
            {
                return BP5-BP3;
            }

            QJsonObject Serialise() const;
            bool Deserialize( QJsonObject const& obj );
        };

        Data():
            Expenditure(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );

        double Expenditure;
        Channel ChA;
        Channel ChB;
    };

    QVector<Data> mData;
    mutable int PrintedRows = 0;
};

}//namespace servo

}//namespace test
