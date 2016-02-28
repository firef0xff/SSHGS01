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
    class GrapfData;
    struct Data
    {
        struct Channel
        {
            Channel():
                Expenditure(0.0),
                BP5(0),
                BP3(0)
            {}
            double Expenditure;
            double BP5;
            double BP3;
            double BP5_3 () const
            {
                return BP5-BP3;
            }

            QJsonObject Serialise() const;
            bool Deserialize( QJsonObject const& obj );

            bool operator == ( const Channel& r ) const
            {
                return Expenditure == r.Expenditure && BP5 == r.BP5  && BP3 == r.BP3;
            }
            bool operator != ( const Channel& r ) const
            {
                return !(*this == r);
            }
        };

        Data()
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );

        bool operator == ( const Data& r ) const
        {
            return ChA == r.ChA && ChB == r.ChB;
        }

        Channel ChA;
        Channel ChB;
    };

    typedef QVector<Data> DataSet;

    ExpeditureFromPressureDuration();
    bool Run();
    bool Success() const;
    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect, const QString &compare_width ) const;

    void ResetDrawLine();
protected:
    void UpdateData();
private:
    friend class GrapfData;

    DataSet mData;
    mutable int PrintedRows = 0;
    mutable int PrintedPage = 0;
    mutable GrapfData* mGrapfs;
};

}//namespace servo

}//namespace test
