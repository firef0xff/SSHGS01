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
    ExpeditureFromInput();
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
        Data():
            Signal(0),
            Expenditure(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
        double Signal;
        double Expenditure;
    };

    // графиг опорного сигнала и расхода в канале
    //+ расчеты
    /// (хочу просто данные с контроллера для рисования )

    QVector<Data> GraphA;
    QVector<Data> GraphB;
    double Gain;            /// коэффициент усиления по расходу
    double Hysteresis;      /// гистерезис
    double Nonlinearity;    /// нелинейность
};

}//namespace servo

}//namespace test
