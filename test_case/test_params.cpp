#include "test_params.h"
#include <memory>
#include <mutex>

namespace test
{

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

