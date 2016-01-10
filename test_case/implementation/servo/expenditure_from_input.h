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
        double PressureBP3;
        double PressureBP4;
        double PressureBP5;
    };
    typedef QVector<Data> DataSet;


    ExpeditureFromInput();
    bool Run();
    bool Success() const;
    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;
protected:
    void UpdateData();
private:


    // графиг опорного сигнала и расхода в канале
    //+ расчеты
    /// (хочу просто данные с контроллера для рисования )

    DataSet GraphA1;
    DataSet GraphA2;
    DataSet GraphB1;
    DataSet GraphB2;

    double Gain;            /// коэффициент усиления по расходу
    double Hysteresis;      /// гистерезис
    double Nonlinearity;    /// нелинейность
};

}//namespace servo

}//namespace test
