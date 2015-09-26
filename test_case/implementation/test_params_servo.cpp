#include "test_params_servo.h"
#include <mutex>
#include <memory>
#include "../tests.h"

namespace test
{

namespace servo
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
    mReelControl( RC_UNKNOWN ),
    mPressureNominal( -1 ),
    mPressureTesting( -1 ),
    mMaxExpenditureA( -1 ),
    mMaxExpenditureB( -1 ),
    mFrequencyInc( -1.0 )
{}

void Parameters::Reset()
{
    test::Parameters::Reset();

    mReelControl = RC_UNKNOWN;
    mPressureNominal = -1;
    mPressureTesting = -1;
    mMaxExpenditureA = -1;
    mMaxExpenditureB = -1;
    mFrequencyInc = -1.0;
}
QString Parameters::ToString()
{
    QString res;
    res+= "Параметры аппаратуры управления направлением расхода:\n";
    res+= "  Серийный номер: " + mSerNo +"\n" ;
    res+= "  Тип управления: " + test::ToString( mReelControl ) + "\n";
    res+= "  Количество катушек питания: " + test::ToString( mReelCount ) + "\n";
    res+= "  Тип управления распределителем: " + test::ToString( mControlType ) + "\n";
    res+= "  Максимальное давление управления, Бар: " + test::ToString( mMaxControlPressure ) + "\n";
    res+= "  Минимальное давление управления, Бар: " + test::ToString( mMinControlPressure ) + "\n";
    res+= "\n";
    res+= "Параметры стенда:\n";
    res+= "  Максимальный расход, л/мин: " + test::ToString( mMaxExpenditure ) + "\n";
    res+= "  Номинальное давление, Бар: " + test::ToString( mPressureNominal ) + "\n";
    res+= "\n";
    res+= "Параметры испытаний:\n";
    res+= "  Давления при испытании аппарата пробным давлением, Бар: " + test::ToString( mPressureTesting ) + "\n";
    res+= "  Максимальный расход в канале А, при следующем опорном сигнале, X_max_A, л/мин: " + test::ToString( mMaxExpenditureA ) + "\n";
    res+= "  Максимальный расход в канале Б, при следующем опорном сигнале, X_max_Б, л/мин: " + test::ToString( mMaxExpenditureB ) + "\n";
    res+= "  Инкремент частоты при построении частотных характеристик, Гц: " + test::ToString( mFrequencyInc ) + "\n";

    return res;
}

test::TestCase& Parameters::TestCollection() const
{
    return ServoTests;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject res = test::CommonParameters::Serialise();
    QJsonObject servo;
    servo.insert("ReelControl", mReelControl);
    servo.insert("PressureNominal", mPressureNominal);
    servo.insert("PressureTesting", mPressureTesting);
    servo.insert("MaxExpenditureA", mMaxExpenditureA);
    servo.insert("MaxExpenditureB", mMaxExpenditureB);
    servo.insert("FrequencyInc", mFrequencyInc);

    res.insert("servo", servo);
    return res;
}
bool Parameters::Deserialize( QJsonObject const& obj )
{
    Reset();
    bool res = test::CommonParameters::Deserialize( obj );

    auto val = obj.value("servo");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();

        mReelControl = static_cast<RELL_CONTROL>(obj.value("ReelControl").toInt());
        mPressureNominal = obj.value("PressureNominal").toInt();
        mPressureTesting = obj.value("PressureTesting").toInt();
        mMaxExpenditureA = obj.value("MaxExpenditureA").toInt();
        mMaxExpenditureB = obj.value("MaxExpenditureB").toInt();
        mFrequencyInc = obj.value("FrequencyInc").toDouble();
        res = true;
    }
    else
        res = false;

    return res;
}

bool Parameters::ReelControl ( QString const& val )
{
    return ParseValue( mReelControl, val );
}
RELL_CONTROL const& Parameters::ReelControl () const
{
    return mReelControl;
}

bool Parameters::PressureNominal ( QString const& val )
{
    return ParseValue( mPressureNominal, val );
}
qint32 const& Parameters::PressureNominal () const
{
    return mPressureNominal;
}

bool Parameters::PressureTesting ( QString const& val )
{
    return ParseValue( mPressureTesting, val );
}
qint32 const& Parameters::PressureTesting () const
{
    return mPressureTesting;
}

bool Parameters::MaxExpenditureA ( QString const& val )
{
    return ParseValue( mMaxExpenditureA, val );
}
qint32 const& Parameters::MaxExpenditureA () const
{
    return mMaxExpenditureA;
}

bool Parameters::MaxExpenditureB ( QString const& val )
{
    return ParseValue( mMaxExpenditureB, val );
}
qint32 const& Parameters::MaxExpenditureB () const
{
    return mMaxExpenditureB;
}

bool Parameters::FrequencyInc ( QString const& val )
{
    return ParseValue( mFrequencyInc, val );
}
const double &Parameters::FrequencyInc() const
{
    return mFrequencyInc;
}

}//namespace servo

}//namespace test
