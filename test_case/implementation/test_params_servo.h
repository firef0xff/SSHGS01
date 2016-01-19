#pragma once
#include "../test_params.h"
#include <array>
namespace test
{
namespace servo
{

class Parameters : public test::CommonParameters
{
public:
    static Parameters& Instance();

    void Reset();
    QString ToString() const;

    virtual test::TestCase& TestCollection() const;

    virtual QJsonObject Serialise() const;
    virtual bool Deserialize( QJsonObject const& obj );

    virtual void WriteToController() const;

    bool Draw(QPainter &painter, QRect &free_rect, QString const& compare_width ) const;

    QString ModelId() const;

    bool GsType ( QString const& val );
    QString const& GsType () const;

    bool ReelControl ( QString const& val );
    RELL_CONTROL const& ReelControl () const;

    bool PressureNominal ( QString const& val );
    double const& PressureNominal () const;

    bool PressureTesting ( QString const& val );
    double const& PressureTesting () const;

    bool MaxExpenditureA ( QString const& val );
    double const& MaxExpenditureA () const;

    bool MaxExpenditureB ( QString const& val );
    double const& MaxExpenditureB () const;

    bool FrequencyInc ( QString const& val );
    const double &FrequencyInc() const;

    bool ControlSignal ( QString const& val );
    const SIGNAL_TYPE &ControlSignal() const;

    bool SignalStateA ( QString const& val );
    const double &SignalStateA() const;

    bool SignalStateB ( QString const& val );
    const double &SignalStateB() const;

    bool SignalState0 ( QString const& val );
    const double &SignalState0() const;

    bool EndSgnal ( QString const& val );
    const double &EndSgnal() const;

    bool ControlReelResist ( QString const& val );
    const double &ControlReelResist() const;

    bool Amplitudes( QString const& val1, QString const& val2, QString const& val3 );
    const std::array< double, 3 > &Amplitudes() const;

    bool Voltage ( QString const& val );
    const double &Voltage() const;

    bool TestChannelA ( bool val );
    const bool &TestChannelA() const;

    bool TestChannelB (bool val );
    const bool &TestChannelB() const;

    bool SignalOnChannelA ( QString const& val );
    const CONTROL_SIGNAL &SignalOnChannelA() const;

    bool SignalOnChannelB ( QString const& val );
    const CONTROL_SIGNAL &SignalOnChannelB() const;
private:
    Parameters();
    Parameters( Parameters const& )  = delete;
    void operator = ( Parameters& ) = delete;

    QString mGsType;            //тип гидрораспределителя
    RELL_CONTROL mReelControl;
    double mPressureNominal;
    double mPressureTesting;
    double mMaxExpenditureA;
    double mMaxExpenditureB;
    double mFrequencyInc;

    SIGNAL_TYPE mControlSignal;
    double mSignalStateA;
    double mSignalStateB;
    double mSignalState0;

    double mEndSgnal;
    double mControlReelResist;

    std::array< double, 3 > mAmplitudes;

    double mVoltage; //  напряжение на блоке управления

    bool mTestChannelA;
    bool mTestChannelB;
    CONTROL_SIGNAL mSignalOnChannelA;
    CONTROL_SIGNAL mSignalOnChannelB;
};

}//namespace servo

}//namespace test

