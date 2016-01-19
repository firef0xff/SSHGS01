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

    void WriteToController() const;

    bool Draw(QPainter &painter, QRect &free_rect, QString const& compare_width ) const;

    bool SerNo ( QString const& val );
    QString const& SerNo () const;

    bool GsType ( QString const& val );
    QString const& GsType () const;

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

    QString mSerNo;         //серийный номер
    QString mGsType;        //тип гидроцилиндра
    double  mMaxPressure;   //максимальное давление нагрузки Г/Ц
    double  mMoveTime;      //время перемещения в каждую сторону
    double  mTestPressure;  //давление при испытании функционирования
    double  mHermTestTime;  //время испытания наружной герметичности
    double  mExpenditure;   //номинальный расход Г/Ц
};
}//namespace hydro_cylinder

}//namespace test

