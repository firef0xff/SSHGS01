#include "test_params_hydro.h"
#include <memory>
#include <mutex>
#include "../tests.h"

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
    mVoltage( -1 ),
    mVoltageRange( -1 ),
    mLost( -1.0 ),
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
    mLost = -1.0;
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
        mVoltageRange =  obj.value("VoltageRange").toInt();
        mLost = obj.value("Lost").toDouble();
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

}//namespace test
