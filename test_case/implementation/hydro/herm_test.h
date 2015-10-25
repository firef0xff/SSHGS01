#pragma once
#include "../test_base.h"
#include <condition_variable>
namespace test
{

namespace hydro
{

class OutsideHermTest :public test::hydro::Test
{
public:
    OutsideHermTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;
private:
    void Question();
    bool LeakFounded; //Течь при испытании наружной герметичности не обнаружена / Испытание внешней герметичности не пройдено
    std::condition_variable mCondVar;
};

class InsideHermTest :public test::hydro::Test
{
public:
    InsideHermTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;
private:
    // Средний расчет расход утечки за %s сек, %s л/мин
    int Seconds;
    double Leak;
    bool Result;
};

}//namespace hydro

}//namespace test
