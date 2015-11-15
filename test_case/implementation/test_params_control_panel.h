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
    QString ToString() const;

    test::TestCase& TestCollection() const;

    QJsonObject Serialise() const;
    bool Deserialize(const QJsonObject &obj );
    bool HasResults()
    {
        return false;
    }

    bool Draw(QPainter &painter, QRect &free_rect ) const;
    QString ModelId() const;

    virtual void WriteToController() const {}

    bool GsType ( QString const& val );
    QString const& GsType () const;

    bool SerNo ( QString const& val );
    QString const& SerNo () const;

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
    QString mSerNo; //серийный номер
    SIGNAL_TYPE mSignalType; //тип сигнала
    double mVoltage;         //напряжение
    double mMaxAmperage;      //максимальный ток
    double mReelResist;       //сопротивление катушки

};
}//namespace control_board

}//namespace test
