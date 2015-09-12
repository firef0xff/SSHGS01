#include "test_params.h"
#include "tests.h"
#include <memory>
#include <mutex>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

namespace test
{

Parameters* CURRENT_PARAMS = nullptr;

bool ParseValue ( CONTROL_SIGNAL& sig, QString const& val )
{
    if ( !val.compare( "Отсутствует", Qt::CaseInsensitive ) )
    {
        sig = CS_NONE;
        return true;
    }
    else if ( !val.compare( "Катушка А", Qt::CaseInsensitive ) )
    {
        sig = CS_REEL_A;
        return true;
    }
    else if ( !val.compare( "Катушка Б", Qt::CaseInsensitive ) )
    {
        sig = CS_REEL_B;
        return true;
    }
    return false;
}
bool ParseValue ( CONTROL_TYPE& sig, QString const& val )
{
    if ( !val.compare( "Электронное", Qt::CaseInsensitive ) )
    {
        sig = CT_ELECTRIC;
        return true;
    }
    else if ( !val.compare( "Электрогидравлическое", Qt::CaseInsensitive ) )
    {
        sig = CT_HYDRO_ELECTRIC;
        return true;
    }
    return false;
}
bool ParseValue ( VOLTAGE_TYPE& sig, QString const& val )
{
    if ( !val.compare( "Постоянное", Qt::CaseInsensitive ) )
    {
        sig = VT_DC;
        return true;
    }
    else if ( !val.compare( "Переменное", Qt::CaseInsensitive ) )
    {
        sig = VT_AC;
        return true;
    }
    return false;
}
bool ParseValue ( qint32& param, QString const& val )
{
    bool t = false;
    auto b =  val.toInt( &t );

    if ( t )
        param = b;
    return t;
}
bool ParseValue ( DYNAMIC& param, QString const& val )
{
    if ( !val.compare( "Увеличение давления", Qt::CaseInsensitive ) )
    {
        param = DN_UP;
        return true;
    }
    else if ( !val.compare( "Уменьшение давления", Qt::CaseInsensitive ) )
    {
        param = DN_DOWN;
        return true;
    }
    return false;
}
bool ParseValue ( DYNAMIC_CONTROL& param, QString const& val )
{
    if ( !val.compare( "ДД1", Qt::CaseInsensitive ) )
    {
        param = DC_DD1;
        return true;
    }
    else if ( !val.compare( "ДД2", Qt::CaseInsensitive ) )
    {
        param = DC_DD2;
        return true;
    }
    else if ( !val.compare( "ДД3", Qt::CaseInsensitive ) )
    {
        param = DC_DD3;
        return true;
    }
    return false;
}
bool ParseValue ( RELL_CONTROL& param, QString const& val )
{
    if ( !val.compare( "Напрямую катушками управления", Qt::CaseInsensitive ) )
    {
        param = RC_REEL;
        return true;
    }
    else if ( !val.compare( "Через блок управления", Qt::CaseInsensitive ) )
    {
        param = RC_CONTROL_BOX;
        return true;
    }
    return false;
}
bool ParseValue ( double_t& param, QString const& val )
{
    bool t = false;
    auto b =  val.toDouble( &t );

    if ( t )
        param = b;
    return t;
}

QString ToString( qint32 const& v )
{
    if ( v >= 0 )
        return QString::number( v );
    else
        return "не задано";
}
QString ToString( VOLTAGE_TYPE const& v )
{
    switch (v)
    {
        case VT_UNKNOWN:
            return "Не задано";
        case VT_AC:
            return "Переменное";
        case VT_DC:
            return "Постоянное";
    default:
        return "Неизвестное значение";
    }
}
QString ToString( CONTROL_TYPE const& v )
{
    switch (v)
    {
        case CT_UNKNOWN:
            return "Не задано";
        case CT_ELECTRIC:
            return "Электронное";
        case CT_HYDRO_ELECTRIC:
            return "Электрогидравлическое";
    default:
        return "Неизвестное значение";
    }
}
QString ToString( CONTROL_SIGNAL const& v )
{
    switch (v)
    {
        case CS_UNKNOWN:
            return "Не задано";
        case CS_NONE:
            return "Отсутствует";
        case CS_REEL_A:
            return "Катушка А";
        case CS_REEL_B:
            return "Катушка Б";
    default:
        return "Неизвестное значение";
    }
}
QString ToString( DYNAMIC const& v )
{
    switch (v)
    {
        case DN_UNKNOWN:
            return "Не задано";
        case DN_UP:
            return "Увеличение давления";
        case DN_DOWN:
            return "Уменьшение давления";
    default:
        return "Неизвестное значение";
    }
}
QString ToString( DYNAMIC_CONTROL const& v )
{
    switch (v)
    {
        case DC_UNKNOWN:
            return "Не задано";
        case DC_DD1:
            return "ДД1";
        case DC_DD2:
            return "ДД2";
        case DC_DD3:
            return "ДД3";
    default:
        return "Неизвестное значение";
    }
}
QString ToString( RELL_CONTROL const& v )
{
    switch (v)
    {
        case RC_UNKNOWN:
            return "Не задано";
        case RC_REEL:
            return "Напрямую катушками управления";
        case RC_CONTROL_BOX:
            return "Через блок управления";
    default:
        return "Неизвестное значение";
    }
}
QString ToString( double_t const& v )
{
    if ( v >= 0 )
        return QString::number( v );
    else
        return "не задано";
}

Parameters::Parameters():
    mSerNo(""),
    mReelCount( -1 ),
    mMaxExpenditure ( -1 ),
    mControlType ( CT_UNKNOWN ),
    mMinControlPressure ( -1 ),
    mMaxControlPressure ( -1 )
{}
void Parameters::Reset()
{
    mSerNo = "";
    mReelCount =  -1;
    mMaxExpenditure  =  -1;
    mControlType  =  CT_UNKNOWN;
    mMinControlPressure  =  -1;
    mMaxControlPressure  =  -1;
    mTestCase.clear();
}

void Parameters::TestCase ( TestsList const& test_case)
{
    mTestCase = test_case;
}
Parameters::TestsList const& Parameters::TestCase ()
{
    return mTestCase;
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

bool Parameters::ReelCount ( QString const& val )
{
    return ParseValue( mReelCount, val );
}
qint32 const& Parameters::ReelCount () const
{
    return mReelCount;
}

bool Parameters::MaxExpenditure ( QString const& val )
{
    return ParseValue( mMaxExpenditure, val );
}
qint32 const& Parameters::MaxExpenditure () const
{
    return mMaxExpenditure;
}

bool Parameters::ControlType ( QString const& val )
{
    return ParseValue( mControlType, val );
}
CONTROL_TYPE const& Parameters::ControlType () const
{
    return mControlType;
}

bool Parameters::MinControlPressure ( QString const& val )
{
    return ParseValue( mMinControlPressure, val );
}
qint32 const& Parameters::MinControlPressure () const
{
    return mMinControlPressure;
}

bool Parameters::MaxControlPressure ( QString const& val )
{
    return ParseValue( mMaxControlPressure, val );
}
qint32 const& Parameters::MaxControlPressure () const
{
    return mMaxControlPressure;
}


QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;
    obj.insert("SerNo", mSerNo);
    obj.insert("ReelCount", mReelCount);
    obj.insert("MaxExpenditure", mMaxExpenditure);
    obj.insert("ControlType",mControlType);
    obj.insert("MinControlPressure",mMinControlPressure);
    obj.insert("MaxControlPressure",mMaxControlPressure);

    QJsonArray tests;
    foreach (test::Test* ptr, mTestCase)
    {
        tests.push_back( ptr->Number() );
    }
    obj.insert("TestCase", tests);
    return obj;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    bool res = true;
    mSerNo = obj.value("SerNo").toString();
    mReelCount = obj.value("ReelCount").toInt();
    mMaxExpenditure = obj.value("MaxExpenditure").toInt();
    mControlType = static_cast<CONTROL_TYPE>( obj.value("ControlType").toInt() );
    mMinControlPressure = obj.value("MinControlPressure").toInt();
    mMaxControlPressure = obj.value("MaxControlPressure").toInt();

    QJsonArray tests = obj.value("TestCase").toArray();

    test::TestCase const& Collection = TestCollection();
    foreach ( QJsonValue const& val, tests )
    {
        bool find = false;
        for ( auto it = Collection.Tests().cbegin(), end = Collection.Tests().cend();
              it != end && !find ; ++it)
        {
            test::Test* ptr = *it;
            if ( ptr->Number() == val.toInt() )
            {
                mTestCase.push_back( ptr );
                find = true;
            }
        }
        res *= find;
    }

    return res;
}


void ToFile( QString fname, Parameters const& params )
{
    QFile f( fname );
    f.open(QIODevice::WriteOnly);
    QJsonDocument doc;
    doc.setObject( params.Serialise() );

    f.write( doc.toJson() );

    f.close();
}
Parameters* FromFile( QString fname )
{
    QFile f( fname );
    Parameters* ret = nullptr;
    if ( f.exists() )
    {
        f.open(QIODevice::ReadOnly);
        auto doc = QJsonDocument::fromJson( f.readAll() );
        QJsonObject obj = doc.object();

        if ( hydro::Parameters::Instance().Deserialize( obj ) )
            ret = &hydro::Parameters::Instance();
        if ( servo::Parameters::Instance().Deserialize( obj ) )
            ret = &servo::Parameters::Instance();
        f.close();
    }

    return ret;
}


namespace hydro
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
    mGsType(""),
    mVoltage( -1 ),
    mVoltageRange( -1 ),
    mVoltageType( VT_UNKNOWN ),       
    mMaxWorkPressure ( -1 ),
    mMinTestPressure ( -1 ),
    mHermPressure( -1 ),
    mHermSignal ( CS_UNKNOWN ),
    mPABTSignal ( CS_UNKNOWN ),
    mPBATSignal ( CS_UNKNOWN ),
    mActuationOnTime ( -1 ),
    mActuationOffTime ( -1 ),
    mOnControl_1 ( DC_UNKNOWN ),
    mOffControl_1 ( DC_UNKNOWN ),
    mOnControl_2 ( DC_UNKNOWN ),
    mOffControl_2 ( DC_UNKNOWN ),
    mOnDynamic_1 ( DN_UNKNOWN ),
    mOffDynamic_1 ( DN_UNKNOWN ),
    mOnDynamic_2 ( DN_UNKNOWN ),
    mOffDynamic_2 ( DN_UNKNOWN )
{
    Reset();
}

void Parameters::Reset()
{    
    test::Parameters::Reset();
    mGsType = "";
    mVoltage =  -1;
    mVoltageRange =  -1;
    mVoltageType = VT_UNKNOWN;    
    mMaxWorkPressure  =  -1;
    mMinTestPressure  =  -1;
    mHermPressure =  -1;
    mHermSignal  =  CS_UNKNOWN;
    mPABTSignal  =  CS_UNKNOWN;
    mPBATSignal  =  CS_UNKNOWN;
    mActuationOnTime  =  -1;
    mActuationOffTime  =  -1;
    mOnControl_1 = DC_UNKNOWN;
    mOffControl_1 = DC_UNKNOWN;
    mOnControl_2 = DC_UNKNOWN;
    mOffControl_2 = DC_UNKNOWN;
    mOnDynamic_1 = DN_UNKNOWN;
    mOffDynamic_1 = DN_UNKNOWN;
    mOnDynamic_2 = DN_UNKNOWN;
    mOffDynamic_2 = DN_UNKNOWN;
}

QString Parameters::ToString()
{
    QString res;
    res+= "Параметры гидрораспределителя:\n";
    res+= "  Серийный номер: " + mSerNo +"\n" ;
    res+= "  Тип: " + mGsType +"\n";
    res+= "  Напряжение питания, В: " + test::ToString( mVoltage ) +"\n";
    res+= "  Тип питающего напряжения: " + test::ToString( mVoltageType ) + "\n";
    res+= "  Допустимое отклонение напрядения питания, %: " + test::ToString( mVoltageRange ) + "\n";
    res+= "  Количество катушек питания: " + test::ToString( mReelCount ) + "\n";
    res+= "  Тип управления: " + test::ToString( mControlType ) + "\n";
    res+= "  Максимальное давление управления, Бар: " + test::ToString( mMaxControlPressure ) + "\n";
    res+= "  Минимальное давление управления, Бар: " + test::ToString( mMinControlPressure ) + "\n";
    res+= "\n";
    res+= "Параметры стенда:\n";
    res+= "  Максимальный расход, л/мин: " + test::ToString( mMaxExpenditure ) + "\n";
    res+= "  Максимальное рабочее давление, Бар: " + test::ToString( mMaxWorkPressure ) + "\n";
    res+= "  Время срабатывания включения распределителя, мсек: " + test::ToString( mActuationOnTime ) + "\n";
    res+= "  Время срабатывания выключения распределителя, мсек: " + test::ToString( mActuationOffTime ) + "\n";
    res+= "\n";
    res+= "Параметры испытаний:\n";
    res+= "  Давление для испытания функционирования минимальным давлением, Бар: " + test::ToString( mMinTestPressure ) + "\n";
    res+= "  Давление для испытания функционирования минимальным давлением, Бар: " + test::ToString( mHermPressure ) + "\n";
    res+= "  Сигнал проверки внутренней герметичности: " + test::ToString( mHermSignal ) + "\n";
    res+= "  Сигнал PABT: " + test::ToString( mPABTSignal ) + "\n";
    res+= "  Сигнал PBAT: " + test::ToString( mPBATSignal ) + "\n";
    res+= "  Диагностика включения катушки:\n";
    res+= "    Катушка А:\n";
    res+= "      Датчик по которому определяется включение: " + test::ToString( mOnControl_1 ) + "\n";
    res+= "      Датчик по которому определяется выключение: " + test::ToString( mOffControl_1 ) + "\n";
    res+= "      Ожидаемая динамика после включения: " + test::ToString( mOnDynamic_1 ) + "\n";
    res+= "      Ожидаемая динамика после выключения: " + test::ToString( mOffDynamic_1 ) + "\n";
    res+= "    Катушка Б:\n";
    res+= "      Датчик по которому определяется включение: " + test::ToString( mOnControl_2 ) + "\n";
    res+= "      Датчик по которому определяется выключение: " + test::ToString( mOffControl_2 ) + "\n";
    res+= "      Ожидаемая динамика после включения: " + test::ToString( mOnDynamic_2 ) + "\n";
    res+= "      Ожидаемая динамика после выключения: " + test::ToString( mOffDynamic_2 ) + "\n";

    return res;
}

test::TestCase const& Parameters::TestCollection() const
{
    return HydroTests;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;
    obj.insert("GsType", mGsType);
    obj.insert("Voltage", mVoltage);
    obj.insert("VoltageRange", mVoltageRange);
    obj.insert("VoltageType", mVoltageType);
    obj.insert("MaxWorkPressure", mMaxWorkPressure);
    obj.insert("MinTestPressure", mMinTestPressure);
    obj.insert("HermPressure", mHermPressure);
    obj.insert("HermSignal", mHermSignal);
    obj.insert("PABTSignal", mPABTSignal);
    obj.insert("PBATSignal", mPBATSignal);
    obj.insert("ActuationOnTime", mActuationOnTime);
    obj.insert("ActuationOffTime", mActuationOffTime);
    obj.insert("OnControl_1", mOnControl_1);
    obj.insert("OffControl_1", mOffControl_1);
    obj.insert("OnControl_2", mOnControl_2);
    obj.insert("OffControl_2", mOffControl_2);
    obj.insert("OnDynamic_1", mOnDynamic_1);
    obj.insert("OffDynamic_1", mOffDynamic_1);
    obj.insert("OnDynamic_2", mOnDynamic_2);
    obj.insert("OffDynamic_2", mOffDynamic_2);

    QJsonObject ret = test::Parameters::Serialise();
    ret.insert("hydro", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    bool ret = test::Parameters::Deserialize( obj );
    auto val = obj.value("hydro");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();
        mGsType = obj.value("GsType").toString();
        mVoltage =  obj.value("Voltage").toInt();
        mVoltageRange =  obj.value("VoltageRange").toInt();
        mVoltageType = static_cast<VOLTAGE_TYPE>( obj.value("VoltageType").toInt() );
        mMaxWorkPressure  =  obj.value("MaxWorkPressure").toInt();
        mMinTestPressure  =  obj.value("MinTestPressure").toInt();
        mHermPressure =  obj.value("HermPressure").toInt();
        mHermSignal  =  static_cast<CONTROL_SIGNAL>( obj.value("HermSignal").toInt() );
        mPABTSignal  =  static_cast<CONTROL_SIGNAL>( obj.value("PABTSignal").toInt() );
        mPBATSignal  =  static_cast<CONTROL_SIGNAL>( obj.value("PBATSignal").toInt() );
        mActuationOnTime  =  obj.value("ActuationOnTime").toInt();
        mActuationOffTime  =  obj.value("ActuationOffTime").toInt();
        mOnControl_1 = static_cast<DYNAMIC_CONTROL>( obj.value("OnControl_1").toInt() );
        mOffControl_1 = static_cast<DYNAMIC_CONTROL>( obj.value("OffControl_1").toInt() );
        mOnControl_2 = static_cast<DYNAMIC_CONTROL>( obj.value("OnControl_2").toInt() );
        mOffControl_2 = static_cast<DYNAMIC_CONTROL>( obj.value("OffControl_2").toInt() );
        mOnDynamic_1 = static_cast<DYNAMIC>( obj.value("OnDynamic_1").toInt() );
        mOffDynamic_1 = static_cast<DYNAMIC>( obj.value("OffDynamic_1").toInt() );
        mOnDynamic_2 = static_cast<DYNAMIC>( obj.value("OnDynamic_2").toInt() );
        mOffDynamic_2 = static_cast<DYNAMIC>( obj.value("OffDynamic_2").toInt() );
        ret = true;
    }
    else
        ret = false;

    return ret;
}

bool Parameters::GsType ( QString const& val )
{
    mGsType = val;
    return true;
}
QString const& Parameters::GsType () const
{
    return mGsType;
}

bool Parameters::Voltage ( QString const& val )
{
    return ParseValue( mVoltage, val );
}
qint32 const& Parameters::Voltage () const
{
    return mVoltage;
}

bool Parameters::VoltageRange ( QString const& val )
{
    return ParseValue( mVoltageRange, val );
}
qint32 const& Parameters::VoltageRange () const
{
    return mVoltageRange;
}

bool Parameters::VoltageType ( QString const& val )
{
    return ParseValue( mVoltageType, val );
}
VOLTAGE_TYPE const& Parameters::VoltageType () const
{
    return mVoltageType;
}


bool Parameters::MaxWorkPressure ( QString const& val )
{
    return ParseValue( mMaxWorkPressure, val );
}
qint32 const& Parameters::MaxWorkPressure () const
{
    return mMaxWorkPressure;
}

bool Parameters::MinTestPressure ( QString const& val )
{
    return ParseValue( mMinTestPressure, val );
}
qint32 const& Parameters::MinTestPressure () const
{
    return mMinTestPressure;
}

bool Parameters::HermPressure ( QString const& val )
{
    return ParseValue( mHermPressure, val );
}
qint32 const& Parameters::HermPressure () const
{
    return mHermPressure;
}

bool Parameters::HermSignal ( QString const& val )
{
    return ParseValue( mHermSignal, val );
}
CONTROL_SIGNAL const& Parameters::HermSignal () const
{
    return mHermSignal;
}

bool Parameters::PABTSignal ( QString const& val )
{
    return ParseValue( mPABTSignal, val );
}
CONTROL_SIGNAL const& Parameters::PABTSignal () const
{
    return mPABTSignal;
}

bool Parameters::PBATSignal ( QString const& val )
{
    return ParseValue( mPBATSignal, val );
}
CONTROL_SIGNAL const& Parameters::PBATSignal () const
{
    return mPBATSignal;
}

bool Parameters::ActuationOnTime ( QString const& val )
{
    return ParseValue( mActuationOnTime, val );
}
qint32 const& Parameters::ActuationOnTime () const
{
    return mActuationOnTime;
}

bool Parameters::ActuationOffTime ( QString const& val )
{
    return ParseValue( mActuationOffTime, val );
}
qint32 const& Parameters::ActuationOffTime () const
{
    return mActuationOffTime;
}

bool Parameters::OnControl_1 ( QString const& val )
{
    return ParseValue( mOnControl_1, val );
}
DYNAMIC_CONTROL const& Parameters::OnControl_1 () const
{
    return mOnControl_1;
}

bool Parameters::OffControl_1 ( QString const& val )
{
    return ParseValue( mOffControl_1, val );
}
DYNAMIC_CONTROL const& Parameters::OffControl_1 () const
{
    return mOffControl_1;
}

bool Parameters::OnControl_2 ( QString const& val )
{
    return ParseValue( mOnControl_2, val );
}
DYNAMIC_CONTROL const& Parameters::OnControl_2 () const
{
    return mOnControl_2;
}

bool Parameters::OffControl_2 ( QString const& val )
{
    return ParseValue( mOffControl_2, val );
}
DYNAMIC_CONTROL const& Parameters::OffControl_2 () const
{
    return mOffControl_2;
}

bool Parameters::OnDynamic_1 ( QString const& val )
{
    return ParseValue( mOnDynamic_1, val );
}
DYNAMIC const& Parameters::OnDynamic_1 () const
{
    return mOnDynamic_1;
}

bool Parameters::OffDynamic_1 ( QString const& val )
{
    return ParseValue( mOffDynamic_1, val );
}
DYNAMIC const& Parameters::OffDynamic_1 () const
{
    return mOffDynamic_1;
}

bool Parameters::OnDynamic_2 ( QString const& val )
{
    return ParseValue( mOnDynamic_2, val );
}
DYNAMIC const& Parameters::OnDynamic_2 () const
{
    return mOnDynamic_2;
}

bool Parameters::OffDynamic_2 ( QString const& val )
{
    return ParseValue( mOffDynamic_2, val );
}
DYNAMIC const& Parameters::OffDynamic_2 () const
{
    return mOffDynamic_2;
}

}//namespace hydro

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

test::TestCase const& Parameters::TestCollection() const
{
    return ServoTests;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject res = test::Parameters::Serialise();
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
    bool res = test::Parameters::Deserialize( obj );

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
const double_t &Parameters::FrequencyInc() const
{
    return mFrequencyInc;
}

}//namespace servo

}//namespace test

