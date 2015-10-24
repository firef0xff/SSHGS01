#include "test_params_hydro.h"
#include <memory>
#include <mutex>
#include "../tests.h"
#include "../../cpu/cpu_memory.h"

namespace test
{

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
    mVoltage( 0.0 ),
    mVoltageRange( 0.0 ),
    mLost( 0),
    mVoltageType( VT_UNKNOWN ),
    mMaxWorkPressure ( 0.0 ),
    mMinTestPressure ( 0.0 ),
    mHermPressure( 0.0 ),
    mTestControlPressure( 0.0 ),
    mHermSignal ( CS_UNKNOWN ),
    mPABTSignal ( CS_UNKNOWN ),
    mPBATSignal ( CS_UNKNOWN ),
    mActuationOnTime ( 0.0 ),
    mActuationOffTime ( 0.0 ),
    mOnDD1A( false ),
    mOnDD2A( false ),
    mOnDD3A( false ),
    mOffDD1A( false ),
    mOffDD2A( false ),
    mOffDD3A( false ),
    mOnDD1B( false ),
    mOnDD2B( false ),
    mOnDD3B( false ),
    mOffDD1B( false ),
    mOffDD2B( false ),
    mOffDD3B( false ),
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
    mVoltage =  0.0;
    mVoltageRange =  0.0;
    mLost = 0;
    mVoltageType = VT_UNKNOWN;
    mMaxWorkPressure  =  0.0;
    mMinTestPressure  =  0.0;
    mHermPressure =  0.0;
    mTestControlPressure = 0.0;
    mHermSignal  =  CS_UNKNOWN;
    mPABTSignal  =  CS_UNKNOWN;
    mPBATSignal  =  CS_UNKNOWN;
    mActuationOnTime  =  0.0;
    mActuationOffTime  =  0.0;

    mOnDD1A = false;
    mOnDD2A = false;
    mOnDD3A = false;
    mOffDD1A = false;
    mOffDD2A = false;
    mOffDD3A = false;
    mOnDD1B = false;
    mOnDD2B = false;
    mOnDD3B = false;
    mOffDD1B = false;
    mOffDD2B = false;
    mOffDD3B = false;

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
    res+= "  Допустимое значение утечки, %: " + test::ToString( mLost ) + "\n";
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
    res+= "  Давление управления для испытания времени прерключения, Бар: " + test::ToString( mTestControlPressure ) + "\n";
    res+= "  Сигнал проверки внутренней герметичности: " + test::ToString( mHermSignal ) + "\n";
    res+= "  Сигнал PABT: " + test::ToString( mPABTSignal ) + "\n";
    res+= "  Сигнал PBAT: " + test::ToString( mPBATSignal ) + "\n";
    res+= "  Диагностика включения катушки:\n";
    res+= "    Катушка А:\n";
    res+= "      Датчики по которым определяется включение: \n";
    res+= "        ДД1:" + test::ToString( mOnDD1A ) + "\n";
    res+= "        ДД2:" + test::ToString( mOnDD2A ) + "\n";
    res+= "        ДД3:" + test::ToString( mOnDD3A ) + "\n";
    res+= "      Датчики по которым определяется выключение: \n";
    res+= "        ДД1:" + test::ToString( mOffDD1A ) + "\n";
    res+= "        ДД2:" + test::ToString( mOffDD2A ) + "\n";
    res+= "        ДД3:" + test::ToString( mOffDD3A ) + "\n";
    res+= "      Ожидаемая динамика после включения: " + test::ToString( mOnDynamic_1 ) + "\n";
    res+= "      Ожидаемая динамика после выключения: " + test::ToString( mOffDynamic_1 ) + "\n";
    res+= "    Катушка Б:\n";
    res+= "      Датчики по которым определяется включение: \n";
    res+= "        ДД1:" + test::ToString( mOnDD1B ) + "\n";
    res+= "        ДД2:" + test::ToString( mOnDD2B ) + "\n";
    res+= "        ДД3:" + test::ToString( mOnDD3B ) + "\n";
    res+= "      Датчики по которым определяется выключение: \n";
    res+= "        ДД1:" + test::ToString( mOffDD1B ) + "\n";
    res+= "        ДД2:" + test::ToString( mOffDD2B ) + "\n";
    res+= "        ДД3:" + test::ToString( mOffDD3B ) + "\n";
    res+= "      Ожидаемая динамика после включения: " + test::ToString( mOnDynamic_2 ) + "\n";
    res+= "      Ожидаемая динамика после выключения: " + test::ToString( mOffDynamic_2 ) + "\n";

    return res;
}

test::TestCase& Parameters::TestCollection() const
{
    return HydroTests;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;
    obj.insert("GsType", mGsType);
    obj.insert("Voltage", mVoltage);
    obj.insert("VoltageRange", mVoltageRange);
    obj.insert("Lost", mLost);
    obj.insert("VoltageType", mVoltageType);
    obj.insert("MaxWorkPressure", mMaxWorkPressure);
    obj.insert("MinTestPressure", mMinTestPressure);
    obj.insert("HermPressure", mHermPressure);
    obj.insert("TestControlPressure", mTestControlPressure);
    obj.insert("HermSignal", mHermSignal);
    obj.insert("PABTSignal", mPABTSignal);
    obj.insert("PBATSignal", mPBATSignal);
    obj.insert("ActuationOnTime", mActuationOnTime);
    obj.insert("ActuationOffTime", mActuationOffTime);

    obj.insert("OnDD1A", mOnDD1A);
    obj.insert("OnDD2A", mOnDD2A);
    obj.insert("OnDD3A", mOnDD3A);
    obj.insert("OffDD1A", mOffDD1A);
    obj.insert("OffDD2A", mOffDD2A);
    obj.insert("OffDD3A", mOffDD3A);
    obj.insert("OnDD1B", mOnDD1B);
    obj.insert("OnDD2B", mOnDD2B);
    obj.insert("OnDD3B", mOnDD3B);
    obj.insert("OffDD1B", mOffDD1B);
    obj.insert("OffDD2B", mOffDD2B);
    obj.insert("OffDD3B", mOffDD3B);

    obj.insert("OnDynamic_1", mOnDynamic_1);
    obj.insert("OffDynamic_1", mOffDynamic_1);
    obj.insert("OnDynamic_2", mOnDynamic_2);
    obj.insert("OffDynamic_2", mOffDynamic_2);

    QJsonObject ret = test::CommonParameters::Serialise();
    ret.insert("hydro", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::CommonParameters::Deserialize( obj );
    auto val = obj.value("hydro");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();
        mGsType = obj.value("GsType").toString();
        mVoltage =  obj.value("Voltage").toInt();
        mVoltageRange =  obj.value("VoltageRange").toDouble();
        mLost = obj.value("Lost").toDouble();
        mVoltageType = static_cast<VOLTAGE_TYPE>( obj.value("VoltageType").toInt() );
        mMaxWorkPressure  =  obj.value("MaxWorkPressure").toDouble();
        mMinTestPressure  =  obj.value("MinTestPressure").toDouble();
        mHermPressure =  obj.value("HermPressure").toDouble();
        mTestControlPressure =  obj.value("TestControlPressure").toDouble();
        mHermSignal  =  static_cast<CONTROL_SIGNAL>( obj.value("HermSignal").toDouble() );
        mPABTSignal  =  static_cast<CONTROL_SIGNAL>( obj.value("PABTSignal").toInt() );
        mPBATSignal  =  static_cast<CONTROL_SIGNAL>( obj.value("PBATSignal").toInt() );
        mActuationOnTime  =  obj.value("ActuationOnTime").toDouble();
        mActuationOffTime  =  obj.value("ActuationOffTime").toDouble();

        mOnDD1A     = obj.value("OnDD1A").toBool();
        mOnDD2A     = obj.value("OnDD2A").toBool();;
        mOnDD3A     = obj.value("OnDD3A").toBool();;
        mOffDD1A    = obj.value("OffDD1A").toBool();;
        mOffDD2A    = obj.value("OffDD2A").toBool();;
        mOffDD3A    = obj.value("OffDD3A").toBool();;
        mOnDD1B     = obj.value("OnDD1B").toBool();;
        mOnDD2B     = obj.value("OnDD2B").toBool();;
        mOnDD3B     = obj.value("OnDD3B").toBool();;
        mOffDD1B    = obj.value("OffDD1B").toBool();;
        mOffDD2B    = obj.value("OffDD2B").toBool();;
        mOffDD3B    = obj.value("OffDD3B").toBool();;

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

void Parameters::WriteToController() const
{
    auto& mem = cpu::CpuMemory::Instance().HydpoParams;

    mem.Current = mVoltageType == VT_DC ? 0: 1;
    mem.Coil = mReelCount == 1 ? 0 : 1;
    mem.TypeControl = mControlType == CT_ELECTRIC ? 0: 1;

    mem.Voltage = mVoltage;
    mem.Delta_U = mVoltageRange;

    mem.Q_max = mMaxExpenditure;
    mem.Q_min = mLost;
    mem.P_max_rab = mMaxWorkPressure;
    mem.P_min = mMinTestPressure;
    mem.P_narug_germ = mHermPressure;

    mem.Signal_vnutr_germ = static_cast< int >(mHermSignal) >= 0 ? static_cast< int >(mHermSignal) : 0;
    mem.Signal_PA_BT = static_cast< int >(mPABTSignal) >= 0 ? static_cast< int >(mPABTSignal) : 0;
    mem.Signal_PB_AT = static_cast< int >(mPBATSignal) >= 0 ? static_cast< int >(mPBATSignal) : 0;

    mem.Time_ON = mActuationOnTime;
    mem.Time_OFF = mActuationOffTime;

    mem.Min_P = mMinControlPressure;
    mem.Max_P = mMaxControlPressure;

    mem.Time_P = mTestControlPressure;
    mem.DD1_open_a = mOnDD1A;
    mem.DD2_open_a = mOnDD2A;
    mem.DD3_open_a = mOnDD3A;
    mem.TypeD_open_a = static_cast< int >(mOnDynamic_1) >= 0 ? static_cast< int >(mOnDynamic_1) : 0;

    mem.DD1_open_b = mOnDD1B;
    mem.DD2_open_b = mOnDD2B;
    mem.DD3_open_b = mOnDD3B;
    mem.TypeD_open_b = static_cast< int >(mOnDynamic_2) >= 0 ? static_cast< int >(mOnDynamic_2) : 0;

    mem.DD1_close_a = mOffDD1A;
    mem.DD2_close_a = mOffDD2A;
    mem.DD3_close_a = mOffDD3A;
    mem.TypeD_close_a = static_cast< int >(mOffDynamic_1) >= 0 ? static_cast< int >(mOffDynamic_1) : 0;

    mem.DD1_close_b = mOffDD1B;
    mem.DD2_close_b = mOffDD2B;
    mem.DD3_close_b = mOffDD3B;
    mem.TypeD_close_b = static_cast< int >(mOffDynamic_2) >= 0 ? static_cast< int >(mOffDynamic_2) : 0;

    mem.Write();
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
double const& Parameters::Voltage () const
{
    return mVoltage;
}

bool Parameters::VoltageRange ( QString const& val )
{
    return ParseValue( mVoltageRange, val );
}
double const& Parameters::VoltageRange () const
{
    return mVoltageRange;
}

bool Parameters::Lost ( QString const& val )
{
    return ParseValue( mLost, val );
}
double const& Parameters::Lost () const
{
    return mLost;
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
double const& Parameters::MaxWorkPressure () const
{
    return mMaxWorkPressure;
}

bool Parameters::MinTestPressure ( QString const& val )
{
    return ParseValue( mMinTestPressure, val );
}
double const& Parameters::MinTestPressure () const
{
    return mMinTestPressure;
}

bool Parameters::HermPressure ( QString const& val )
{
    return ParseValue( mHermPressure, val );
}
double const& Parameters::HermPressure () const
{
    return mHermPressure;
}

bool Parameters::TestControlPressure ( QString const& val )
{
    return ParseValue( mTestControlPressure, val );
}
const double &Parameters::TestControlPressure() const
{
    return mTestControlPressure;
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
double const& Parameters::ActuationOnTime () const
{
    return mActuationOnTime;
}

bool Parameters::ActuationOffTime ( QString const& val )
{
    return ParseValue( mActuationOffTime, val );
}
const double &Parameters::ActuationOffTime() const
{
    return mActuationOffTime;
}

bool Parameters::OnDD1A ( Qt::CheckState const& val )
{
    mOnDD1A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD1A () const
{
    return mOnDD1A;
}
bool Parameters::OnDD2A ( Qt::CheckState const& val )
{
    mOnDD2A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD2A () const
{
    return mOnDD2A;
}
bool Parameters::OnDD3A ( Qt::CheckState const& val )
{
    mOnDD3A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD3A () const
{
    return mOnDD3A;
}

bool Parameters::OffDD1A ( Qt::CheckState const& val )
{
    mOffDD1A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD1A () const
{
    return mOffDD1A;
}
bool Parameters::OffDD2A ( Qt::CheckState const& val )
{
    mOffDD2A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD2A () const
{
    return mOffDD2A;
}
bool Parameters::OffDD3A ( Qt::CheckState const& val )
{
    mOffDD3A = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD3A () const
{
    return mOffDD3A;
}

bool Parameters::OnDD1B ( Qt::CheckState const& val )
{
    mOnDD1B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD1B () const
{
    return mOnDD1B;
}
bool Parameters::OnDD2B ( Qt::CheckState const& val )
{
    mOnDD2B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD2B () const
{
    return mOnDD2B;
}
bool Parameters::OnDD3B ( Qt::CheckState const& val )
{
    mOnDD3B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OnDD3B () const
{
    return mOnDD3B;
}

bool Parameters::OffDD1B ( Qt::CheckState const& val )
{
    mOffDD1B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD1B () const
{
    return mOffDD1B;
}
bool Parameters::OffDD2B ( Qt::CheckState const& val )
{
    mOffDD2B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD2B () const
{
    return mOffDD2B;
}
bool Parameters::OffDD3B ( Qt::CheckState const& val )
{
    mOffDD3B = val != Qt::Unchecked;
    return true;
}
bool const& Parameters::OffDD3B () const
{
    return mOffDD3B;
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
