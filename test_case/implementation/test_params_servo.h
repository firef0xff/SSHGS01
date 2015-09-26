#pragma once
#include "../test_params.h"

namespace test
{
namespace servo
{

class Parameters : public test::CommonParameters
{
public:
    static Parameters& Instance();

    void Reset();
    QString ToString();

    virtual test::TestCase& TestCollection() const;

    virtual QJsonObject Serialise() const;
    virtual bool Deserialize( QJsonObject const& obj );

    bool ReelControl ( QString const& val );
    RELL_CONTROL const& ReelControl () const;

    bool PressureNominal ( QString const& val );
    qint32 const& PressureNominal () const;

    bool PressureTesting ( QString const& val );
    qint32 const& PressureTesting () const;

    bool MaxExpenditureA ( QString const& val );
    qint32 const& MaxExpenditureA () const;

    bool MaxExpenditureB ( QString const& val );
    qint32 const& MaxExpenditureB () const;

    bool FrequencyInc ( QString const& val );
    const double &FrequencyInc() const;

private:
    Parameters();
    Parameters( Parameters const& )  = delete;
    void operator = ( Parameters& ) = delete;

    RELL_CONTROL mReelControl;
    qint32 mPressureNominal;
    qint32 mPressureTesting;
    qint32 mMaxExpenditureA;
    qint32 mMaxExpenditureB;
    double mFrequencyInc;
};

}//namespace servo

}//namespace test
