#pragma once
#include "../test_base.h"

namespace test
{

namespace hydro
{

class FunctionalTest :public test::hydro::Test
{
public:
    FunctionalTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );
private:
    struct ReelResult
    {
        ReelResult():
            work_on_min_pressure(false),
            work_on_max_pressure(false),
            I(0),
            U(0),
            R(0),
            P(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );

        bool work_on_min_pressure; //Функционирование при минимальном давлении : Да/Нет
        bool work_on_max_pressure; //Функционирование при максимальном давлении : Да/Нет
        //Характеристики
        double I; //Ток I, А
        double U; //Напряжение U, В
        double R; //Сопротивление R, Ом
        double P; //Мощьность P, Ват
    };

    //Катушка А:
    ReelResult ReelA;
    //Катушка Б:
    ReelResult ReelB;
};

}//namespace hydro

}//namespace test
