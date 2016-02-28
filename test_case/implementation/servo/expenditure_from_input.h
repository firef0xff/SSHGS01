#pragma once
#include "../test_base.h"
#include <QVector>

namespace test
{

namespace servo
{
class ExpeditureFromInput :public test::servo::Test
{
public:
    class GrapfData;
    struct Data
    {
        Data():
            Signal(0),
            Expenditure(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
        double Signal;
        double Expenditure;
    };
    typedef QVector<Data> DataSet;


    ExpeditureFromInput();
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

    // графиг опорного сигнала и расхода в канале
    //+ расчеты
    /// (хочу просто данные с контроллера для рисования )

    DataSet GraphA1;
    DataSet GraphA2;
    DataSet GraphB1;
    DataSet GraphB2;

    mutable GrapfData* mGrapfs;
    int level;
};

}//namespace servo

}//namespace test
