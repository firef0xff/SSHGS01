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
    QString ToString() const;

    test::TestCase& TestCollection() const;

    QJsonObject Serialise() const;
    bool Deserialize(const QJsonObject &obj );
    bool HasResults()
    {
        return false;
    }

    virtual void WriteToController() const {}

    bool Draw(QPainter &painter, QRect &free_rect ) const;

    bool SerNo ( QString const& val );
    QString const& SerNo () const;

    bool MaxPressure ( QString const& val );
    double const& MaxPressure () const;

    bool MoveTime ( QString const& val );
    const double &MoveTime() const;

    bool TestPressure ( QString const& val );
    double const& TestPressure () const;

    bool HermTestTime ( QString const& val );
    double const& HermTestTime () const;

    bool Expenditure ( QString const& val );
    double const& Expenditure () const;

private:
    Parameters();
    Parameters( Parameters const& ) = delete;
    void operator = ( Parameters& ) = delete;

    QString mSerNo;
    double  mMaxPressure;
    double  mMoveTime;
    double  mTestPressure;
    double  mHermTestTime;
    double  mExpenditure;
};
}//namespace hydro_cylinder

}//namespace test

