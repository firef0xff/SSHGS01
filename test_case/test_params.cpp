#include "test_params.h"
#include <memory>
#include <mutex>

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

namespace
{
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
            return "не задано";
        case VT_AC:
            return "переменное";
        case VT_DC:
            return "постоянное";
    default:
        return "неизвестное значение";
    }
}
QString ToString( CONTROL_TYPE const& v )
{
    switch (v)
    {
        case CT_UNKNOWN:
            return "не задано";
        case CT_ELECTRIC:
            return "электронное";
        case CT_HYDRO_ELECTRIC:
            return "электрогидравлическое";
    default:
        return "неизвестное значение";
    }
}
QString ToString( CONTROL_SIGNAL const& v )
{
    switch (v)
    {
        case CS_UNKNOWN:
            return "не задано";
        case CS_NONE:
            return "отсутствует";
        case CS_REEL_A:
            return "катушка А";
        case CS_REEL_B:
            return "катушка Б";
    default:
        return "неизвестное значение";
    }
}
QString ToString( DYNAMIC const& v )
{
    switch (v)
    {
        case DN_UNKNOWN:
            return "не задано";
        case DN_UP:
            return "увеличение давления";
        case DN_DOWN:
            return "уменьшение давления";
    default:
        return "неизвестное значение";
    }
}
QString ToString( DYNAMIC_CONTROL const& v )
{
    switch (v)
    {
        case DC_UNKNOWN:
            return "не задано";
        case DC_DD1:
            return "ДД1";
        case DC_DD2:
            return "ДД2";
        case DC_DD3:
            return "ДД3";
    default:
        return "неизвестное значение";
    }
}
}//namespace

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
    mSerNo(""),
    mGsType(""),
    mVoltage( -1 ),
    mVoltageRange( -1 ),
    mVoltageType( VT_UNKNOWN ),
    mReelCount( -1 ),
    mMaxExpenditure ( -1 ),
    mMaxWorkPressure ( -1 ),
    mMinTestPressure ( -1 ),
    mHermPressure( -1 ),
    mHermSignal ( CS_UNKNOWN ),
    mPABTSignal ( CS_UNKNOWN ),
    mPBATSignal ( CS_UNKNOWN ),
    mActuationOnTime ( -1 ),
    mActuationOffTime ( -1 ),
    mControlType ( CT_UNKNOWN ),
    mMinControlPressure ( -1 ),
    mMaxControlPressure ( -1 ),
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
    mSerNo = "";
    mGsType = "";
    mVoltage =  -1;
    mVoltageRange =  -1;
    mVoltageType = VT_UNKNOWN;
    mReelCount =  -1;
    mMaxExpenditure  =  -1;
    mMaxWorkPressure  =  -1;
    mMinTestPressure  =  -1;
    mHermPressure =  -1;
    mHermSignal  =  CS_UNKNOWN;
    mPABTSignal  =  CS_UNKNOWN;
    mPBATSignal  =  CS_UNKNOWN;
    mActuationOnTime  =  -1;
    mActuationOffTime  =  -1;
    mControlType  =  CT_UNKNOWN;
    mMinControlPressure  =  -1;
    mMaxControlPressure  =  -1;
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

bool Parameters::SerNo ( QString const& val )
{
    mSerNo = val;
    return true;
}
QString const& Parameters::SerNo () const
{
    return mSerNo;
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

}//namespace test

