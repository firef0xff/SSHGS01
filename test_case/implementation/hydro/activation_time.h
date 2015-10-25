#pragma once
#include "../test_base.h"

namespace test
{

namespace hydro
{

class ActivationTime :public test::hydro::Test
{
public:
    ActivationTime();
    bool Run();

    QJsonObject Serialise() const;
    bool Deserialize( QJsonObject const& obj );

    bool Draw(QPainter &painter, QRect &free_rect ) const;
private:
    class Data
    {
    public:
        Data():
            TimeOn(0),
            TimeOff(0),
            InTimeOn(false),
            InTimeOff(false),
            IsOn(false),
            IsOff(false)
        {}
        QJsonObject Serialise() const;
        bool Deserialize( QJsonObject const& obj );

        double TimeOn;
        double TimeOff;
        bool InTimeOn;
        bool InTimeOff;
        bool IsOn;
        bool IsOff;
    };

    Data ReelA;
    Data ReelB;
};


}//namespace hydro

}//namespace test
