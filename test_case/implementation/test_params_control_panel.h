#pragma once
#include "../test_params.h"

namespace test
{
namespace control_board
{
class Parameters : public test::Parameters
{
public:
    static Parameters& Instance();

    void Reset();
    QString ToString();

    test::TestCase& TestCollection() const;

    QJsonObject Serialise() const;
    bool Deserialize(const QJsonObject &obj );

    bool GsType ( QString const& val );
    QString const& GsType () const;

    bool SignalType ( QString const& val );
    SIGNAL_TYPE const& SignalType () const;

    bool Voltage ( QString const& val );
    double const& Voltage () const;

    bool MaxAmperage ( QString const& val );
    double const& MaxAmperage () const;

    bool ReelResist ( QString const& val );
    double const& ReelResist () const;
private:
    Parameters();
    Parameters( Parameters const& ) = delete;
    void operator = ( Parameters& ) = delete;

    QString mGsType; //тип устройства
    SIGNAL_TYPE mSignalType; //тип сигнала
    double mVoltage;         //напряжение
    double mMaxAmperage;      //максимальный ток
    double mReelResist;       //сопротивление катушки

};
}//namespace control_board

}//namespace test
