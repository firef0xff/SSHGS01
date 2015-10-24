#pragma once
#include "../test_params.h"

namespace test
{
namespace hydro_cylinder
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

    virtual void WriteToController() const {}

    bool SerNo ( QString const& val );
    QString const& SerNo () const;

    bool MaxPressure ( QString const& val );
    qint32 const& MaxPressure () const;

    bool MoveTime ( QString const& val );
    const double &MoveTime() const;

    bool TestPressure ( QString const& val );
    qint32 const& TestPressure () const;

    bool HermTestTime ( QString const& val );
    qint32 const& HermTestTime () const;

    bool Expenditure ( QString const& val );
    qint32 const& Expenditure () const;

private:
    Parameters();
    Parameters( Parameters const& ) = delete;
    void operator = ( Parameters& ) = delete;

    QString mSerNo;
    qint32  mMaxPressure;
    double  mMoveTime;
    qint32  mTestPressure;
    qint32  mHermTestTime;
    qint32  mExpenditure;
};
}//namespace hydro_cylinder

}//namespace test

