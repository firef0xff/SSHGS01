#include "test_params_control_panel.h"
#include <mutex>
#include <memory>
#include "../tests.h"

namespace test
{
namespace control_board
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
    mSignalType( ST_UNKNOWN ),
    mVoltage( 0 ),
    mMaxAmperage( 0 ),
    mReelResist( 0 )
{
    Reset();
}

void Parameters::Reset()
{
    test::Parameters::Reset();

    mGsType = "";
    mSignalType = ST_UNKNOWN;
    mVoltage = 0;
    mMaxAmperage = 0;
    mReelResist = 0;
}

QString Parameters::ToString() const
{
    QString res;
    res+= "Параметры платы:\n";
    res+= "  Тип: " + mGsType +"\n";
    res+= "  Тип входного сигнала: " + test::ToString( mVoltage ) +"\n";
    res+= "  Напряжение питания, В: " + test::ToString( mSignalType ) +"\n";
    res+= "  Максимальный выходной ток, А: " + test::ToString( mMaxAmperage ) +"\n";
    res+= "  Сопротивление катушки распределителя, Ом: " + test::ToString( mReelResist ) +"\n";
    res+= "\n";

    return res;
}

test::TestCase& Parameters::TestCollection() const
{
    return ControlBoardTests;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;

    obj.insert("GsType", mGsType);
    obj.insert("Voltage", mVoltage);
    obj.insert("SignalType", mSignalType);
    obj.insert("MaxAmperage", mMaxAmperage);
    obj.insert("ReelResist", mReelResist);

    QJsonObject ret = test::Parameters::Serialise();
    ret.insert("control_board", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::Parameters::Deserialize( obj );
    auto val = obj.value("control_board");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();

        mGsType  =  obj.value("GsType").toString();
        mVoltage  =  obj.value("Voltage").toDouble();
        mSignalType  =  static_cast<SIGNAL_TYPE>( obj.value("SignalType").toInt() );
        mMaxAmperage  =  obj.value("MaxAmperage").toDouble();
        mReelResist  =  obj.value("ReelResist").toDouble();

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

bool Parameters::SignalType ( QString const& val )
{
    return ParseValue( mSignalType, val );
}
SIGNAL_TYPE const& Parameters::SignalType () const
{
    return mSignalType;
}

bool Parameters::Voltage ( QString const& val )
{
    return ParseValue( mVoltage, val );
}
double const& Parameters::Voltage () const
{
    return mVoltage;
}

bool Parameters::MaxAmperage ( QString const& val )
{
    return ParseValue( mMaxAmperage, val );
}
double const& Parameters::MaxAmperage () const
{
    return mMaxAmperage;
}

bool Parameters::ReelResist ( QString const& val )
{
    return ParseValue( mReelResist, val );
}
double const& Parameters::ReelResist () const
{
    return mReelResist;
}

}//namespace control_board
}//namespace test
