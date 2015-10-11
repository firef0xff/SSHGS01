#pragma once
#include "../test_base.h"
#include <QVector>


namespace test
{
namespace hydro
{

class PressureDurationFromExpenditure :public test::hydro::Test
{
public:
    PressureDurationFromExpenditure();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    void ResetDrawLine();
    bool Draw(QPainter &painter, QRect &free_rect ) const;
private:
    struct Data
    {
        Data():
            BP5(0),
            BP3(0),
            BP5_3(0),
            Expenditure(0)
        {}
        double BP5;
        double BP3;
        double BP5_3;
        double Expenditure;

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
    };
    typedef std::pair<Data, Data> Channels;

    QVector< Channels > mData;

    mutable int mRowsPrinted;

    //Зависимость перепада давления от расхода
    /// |--------------------------------------------|--------------------------------------------|
    /// |                  Канал А                   |                  Канал Б                   |
    /// |---------|---------|---------------|--------|---------|---------|---------------|--------|
    /// | Давление| Давление| Перепад       | Расход,| Давление| Давление| Перепад       | Расход,|
    /// | БP5, Бар| БР3, Бар| БР5 - БР3, Бар| л/мин  | БP5, Бар| БР3, Бар| БР5 - БР3, Бар| л/мин  |
    /// |---------|---------|---------------|--------|---------|---------|---------------|--------|
    /// |   %ld   |   %ld   |     %ld       |  %ld   |   %ld   |   %ld   |      %ld      |  %ld   |
    /// |---------|---------|---------------|--------|---------|---------|---------------|--------|

};

}//namespace hydro

}//namespace test
