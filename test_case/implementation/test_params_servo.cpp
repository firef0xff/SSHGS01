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
    mPressureNominal( 0 ),
    mPressureTesting( 0 ),
    mMaxExpenditureA( 0 ),
    mMaxExpenditureB( 0 ),
    mFrequencyInc( 0 ),
    mControlSignal(ST_UNKNOWN),
    mSignalStateA(0),
    mSignalStateB(0),
    mSignalState0(0),
    mEndSgnal(0),
    mControlReelResist(0)
{}

void Parameters::Reset()
{
    test::Parameters::Reset();

    mReelControl = RC_UNKNOWN;
    mPressureNominal = 0;
    mPressureTesting = 0;
    mMaxExpenditureA = 0;
    mMaxExpenditureB = 0;
    mFrequencyInc = 0;

    mControlSignal = ST_UNKNOWN;
    mSignalStateA = 0;
    mSignalStateB = 0;
    mSignalState0 = 0;

    mEndSgnal = 0;
    mControlReelResist = 0;
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
    res+= "  Уровень управляющих сигналов: " + test::ToString( mControlSignal ) + "\n";
    res+= "Парамеры для аппаратуры с типом управления: " + test::ToString( RC_CONTROL_BOX );
    res+= "  Сигнал, соответствующий полному переключению в состояние А: " + test::ToString( mSignalStateA ) + "\n";
    res+= "  Сигнал, соответствующий полному переключению в состояние Б: " + test::ToString( mSignalStateB ) + "\n";
    res+= "  Сигнал, соответствующий нулевому положению: " + test::ToString( mSignalState0 ) + "\n";
    res+= "Парамеры для аппаратуры с типом управления: " + test::ToString( RC_REEL );
    res+= "  Сигнал, соответствующий переключению аппарата в конечное состоние: " + test::ToString( mEndSgnal ) + "\n";
    res+= "  Сопротивление катушки управления, Ом: " + test::ToString( mSignalState0 ) + "\n";
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

    servo.insert("ControlSignal", mControlSignal);
    servo.insert("SignalStateA", mSignalStateA);
    servo.insert("SignalStateB", mSignalStateB);
    servo.insert("SignalState0", mSignalState0);

    servo.insert("EndSgnal", mEndSgnal);
    servo.insert("ControlReelResist", mControlReelResist);

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

        mControlSignal = static_cast<SIGNAL_TYPE>(obj.value("ControlSignal").toInt());
        mSignalStateA = obj.value("SignalStateA").toDouble();
        mSignalStateB = obj.value("SignalStateB").toDouble();
        mSignalState0 = obj.value("SignalState0").toDouble();

        mEndSgnal = obj.value("EndSgnal").toDouble();
        mControlReelResist = obj.value("ControlReelResist").toDouble();
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

bool Parameters::ControlSignal ( QString const& val )
{
    return ParseValue( mControlSignal, val );
}
const SIGNAL_TYPE &Parameters::ControlSignal() const
{
    return mControlSignal;
}

bool Parameters::SignalStateA ( QString const& val )
{
    return ParseValue( mSignalStateA, val );
}
const double &Parameters::SignalStateA() const
{
    return mSignalStateA;
}

bool Parameters::SignalStateB ( QString const& val )
{
    return ParseValue( mSignalStateB, val );
}
const double &Parameters::SignalStateB() const
{
    return mSignalStateB;
}

bool Parameters::SignalState0 ( QString const& val )
{
    return ParseValue( mSignalState0, val );
}
const double &Parameters::SignalState0() const
{
    return mSignalState0;
}

bool Parameters::EndSgnal ( QString const& val )
{
    return ParseValue( mEndSgnal, val );
}
const double &Parameters::EndSgnal() const
{
    return mEndSgnal;
}

bool Parameters::ControlReelResist ( QString const& val )
{
    return ParseValue( mControlReelResist, val );
}
const double &Parameters::ControlReelResist() const
{
    return mControlReelResist;
}

}//namespace servo

}//namespace test
