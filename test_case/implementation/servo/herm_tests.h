#pragma once
#include "../test_base.h"
#include <QVector>

namespace test
{

namespace servo
{

class OutsideHermTest :public test::servo::Test
{
public:
    OutsideHermTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );
private:
    void Question();
    bool LeakFounded; //наружная течь (не)замечена -- диалоговое окно
};

class InsideHermTest :public test::servo::Test
{
public:
    InsideHermTest();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );
private:
    struct Data
    {
        Data():
            Signal(0),
            Leak(0)
        {}

        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );
        double Signal;
        double Leak;
    };
    // графиг опорного сигнала и расхода в канале утечки ( Т )

    QVector<Data> Graph;
};

}//namespace servo

}//namespace test
