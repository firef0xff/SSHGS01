#include "test_params_hydro_cilinder.h"
#include "../tests.h"
#include <memory>
#include <mutex>

namespace test
{
namespace hydro_cylinder
{
static std::shared_ptr< Parameters > PARAMS_INSTANCE;
static std::mutex PARAMS_INIT_MUTEX;


Parameters& Parameters::Instance()
{
    if ( !PARAMS_INSTANCE )
    {
        std::lock_guard< std::mutex > lock( PARAMS_INIT_MUTEX );
        if ( !PARAMS_INSTANCE )
        {
            PARAMS_INSTANCE.reset( new Parameters() );
        }
    }

    return *PARAMS_INSTANCE.get();
}
Parameters::Parameters():
    mSerNo(""),
    mMaxPressure(0),
    mMoveTime(0),
    mTestPressure(0),
    mHermTestTime(0),
    mExpenditure(0)
{
    Reset();
}

void Parameters::Reset()
{
    test::Parameters::Reset();
    mSerNo = "";
    mMaxPressure = 0;
    mMoveTime = 0;
    mTestPressure = 0;
    mHermTestTime = 0;
    mExpenditure = 0;
}

QString Parameters::ToString() const
{
    QString res;
    res+= "Параметры гидроцилиндра:\n";
    res+= "  Серийниый номер: " + mSerNo +"\n";
    res+= "  Максимальное давление нагрузки, Бар: " + test::ToString( mMaxPressure ) +"\n";
    res+= "  Номинальный расход, л/мин: " + test::ToString( mExpenditure ) +"\n";
    res+= "Параметры испытаний:\n";
    res+= "  Давление при испытании функционирования, Бар: " + test::ToString( mTestPressure ) +"\n";
    res+= "  Время перемещения в каждую сторону при испытании функционирования, с: " + test::ToString( mMoveTime ) +"\n";
    res+= "  Время испытания наружной герметичности, с: " + test::ToString( mHermTestTime ) +"\n";
    res+= "\n";

    return res;
}

test::TestCase& Parameters::TestCollection() const
{
    return HydroCylinder;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;

    QJsonObject ret = test::Parameters::Serialise();
    obj.insert("SerNo", mSerNo);
    obj.insert("MaxPressure", mMaxPressure);
    obj.insert("MoveTime", mMoveTime);
    obj.insert("TestPressure",mTestPressure);
    obj.insert("HermTestTime",mHermTestTime);
    obj.insert("Expenditure",mExpenditure);

    ret.insert("hydro_cylinder", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::Parameters::Deserialize( obj );
    auto val = obj.value("hydro_cylinder");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();

        mSerNo = obj.value("SerNo").toString();
        mMaxPressure = obj.value("MaxPressure").toInt();
        mMoveTime = obj.value("MoveTime").toDouble();
        mTestPressure = obj.value("TestPressure").toInt();
        mHermTestTime = obj.value("HermTestTime").toInt();
        mExpenditure = obj.value("Expenditure").toInt();

        ret = true;
    }
    else
        ret = false;

    return ret;
}

bool Parameters::SerNo ( QString const& val )
{
    mSerNo = val;
    return true;
}
QString const& Parameters::SerNo () const
{
    return mSerNo;
}

bool Parameters::MaxPressure ( QString const& val )
{
    return ParseValue( mMaxPressure, val );
}
qint32 const& Parameters::MaxPressure () const
{
    return mMaxPressure;
}

bool Parameters::MoveTime ( QString const& val )
{
    return ParseValue( mMoveTime, val );
}
double const& Parameters::MoveTime () const
{
    return mMoveTime;
}

bool Parameters::TestPressure ( QString const& val )
{
    return ParseValue( mTestPressure, val );
}
qint32 const& Parameters::TestPressure () const
{
    return mTestPressure;
}

bool Parameters::HermTestTime ( QString const& val )
{
    return ParseValue( mHermTestTime, val );
}
qint32 const& Parameters::HermTestTime () const
{
    return mHermTestTime;
}

bool Parameters::Expenditure ( QString const& val )
{
    return ParseValue( mExpenditure, val );
}
qint32 const& Parameters::Expenditure () const
{
    return mExpenditure;
}

}//namespace hydro_cylinder

}//namespace test
