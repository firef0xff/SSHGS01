#include "test_params_pumps.h"
#include <memory>
#include <mutex>
#include "../tests.h"
#include "../../cpu/cpu_memory.h"

namespace test
{
bool ParseValue ( SPIN& sig, QString const& val )
{
   if ( !val.compare( "Левое", Qt::CaseInsensitive ) )
   {
       sig = spLeft;
       return true;
   }
   else if ( !val.compare( "Правое", Qt::CaseInsensitive ) )
   {
       sig = spRight;
       return true;
   }
   return false;
}
QString ToString( SPIN const& v )
{
   switch (v)
   {
       case spUnknown:
           return "Не задано";
       case spLeft:
           return "Левое";
       case spRight:
           return "Правое";
   default:
       return "Неизвестное значение";
   }
}

namespace pump
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

Parameters::Parameters()
{
   Reset();
}

void Parameters::Reset()
{
   test::Parameters::Reset();

}

QString Parameters::ToString() const
{
   QString res;
   return res;
}

test::TestCase& Parameters::TestCollection() const
{
   return Pumps;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;
//    obj.insert("SerianNo", mSerianNo);
//    obj.insert("Spin", mSpin);
//    obj.insert("WorkVolume", mWorkVolume);
//    obj.insert("PressureMin", mPressureMin);
//    obj.insert("PressureNom", mPressureNom);
//    obj.insert("PressureMax", mPressureMax);
//    obj.insert("SpinSpeedMin", mSpinSpeedMin);
//    obj.insert("SpinSpeedNom", mSpinSpeedNom);
//    obj.insert("SpinSpeedMax", mSpinSpeedMax);
//    obj.insert("VolumeKPD", mVolumeKPD);
//    obj.insert("Expenditure", mExpenditure);

    QJsonObject ret = test::Parameters::Serialise();
    ret.insert("pump", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::Parameters::Deserialize( obj );
    auto val = obj.value("pupm");
    if ( val.isObject() )
    {
//        QJsonObject obj = val.toObject();
//        mSerianNo = obj.value("SerianNo").toString();
//        mSpin =  static_cast<SPIN>( obj.value("Spin").toInt() );
//        mWorkVolume =  obj.value("WorkVolume").toDouble();

//        mPressureMin = obj.value("PressureMin").toDouble();
//        mPressureNom = obj.value("PressureNom").toDouble();
//        mPressureMax = obj.value("PressureMax").toDouble();

//        mSpinSpeedMin = obj.value("SpinSpeedMin").toInt();
//        mSpinSpeedNom = obj.value("SpinSpeedNom").toInt();
//        mSpinSpeedMax = obj.value("SpinSpeedMax").toInt();

//        mVolumeKPD = obj.value("VolumeKPD").toDouble();
//        mExpenditure = obj.value("Expenditure").toDouble();
    }
    else
        ret = false;

    return ret;
}

void Parameters::WriteToController() const
{
//TODO
}

bool Parameters::Draw(QPainter &painter, QRect &free_rect, QString const& compare_width ) const
{
   //TODO
    return true;
}

bool Parameters::DrawResults(QPainter &painter, QRect &free_rect ) const
{
   //TODO
    return true;
}

QString Parameters::ModelId() const
{
    return mSerianNo;
}

//------------------------------------------------------------------------------------------------
bool Parameters::SerianNo( QString const& val )
{
   mSerianNo = val;
   return true;
}
QString const& Parameters::SerianNo()
{
   return mSerianNo;
}
bool Parameters::Model( QString const& val )
{
   mModel = val;
   return !mModel.isEmpty();
}
QString const& Parameters::Model()
{
   return mModel;
}
//Ответственный User()
bool Parameters::SectionsCount( qint32 val )
{
   mSectionsCount = val;
   return true;
}
qint32 Parameters::SectionsCount()
{
   return mSectionsCount;
}
bool Parameters::Spin( QString const& val )
{
   return ParseValue( mSpin, val );
}
SPIN Parameters::Spin()
{
   return mSpin;
}

bool Parameters::ElektrikControl( bool val )
{
   mElektrikControl = val;
   return true;
}
bool Parameters::ElektrikControl()
{
   return mElektrikControl;
}
bool Parameters::Voltage( QString const& val )
{
   return ParseValue( mVoltage, val );
}
double Parameters::Voltage()
{
   return mVoltage;
}
bool Parameters::VoltageType( QString const& val )
{
   return ParseValue( mVoltageType, val );
}
VOLTAGE_TYPE Parameters::VoltageType()
{
   return mVoltageType;
}

bool Parameters::HydroControl( bool val )
{
   mHydroControl = val;
   return true;
}
bool Parameters::HydroControl()
{
   return mHydroControl;
}
bool Parameters::MaxControlPressure( QString const& val )
{
   return ParseValue( mMaxControlPressure, val );
}
double Parameters::MaxControlPressure()
{
   return mMaxControlPressure;
}

bool     Parameters::WorkVolume1( QString const& val )
{
   return ParseValue( mWorkVolume1, val );
}
double   Parameters::WorkVolume1()
{
   return mWorkVolume1;
}
bool     Parameters::PressureMin1( QString const& val )
{
   return ParseValue( mPressureMin1, val );
}
double   Parameters::PressureMin1()
{
   return mPressureMin1;
}
bool     Parameters::PressureNom1( QString const& val )
{
   return ParseValue( mPressureNom1, val );
}
double   Parameters::PressureNom1()
{
   return mPressureNom1;
}
bool     Parameters::PressureMax1( QString const& val )
{
   return ParseValue( mPressureMax1, val );
}
double   Parameters::PressureMax1()
{
   return mPressureMax1;
}

bool     Parameters::WorkVolume2( QString const& val )
{
   return ParseValue( mWorkVolume2, val );
}
double   Parameters::WorkVolume2()
{
   return mWorkVolume2;
}
bool     Parameters::PressureMin2( QString const& val )
{
   return ParseValue( mPressureMin2, val );
}
double   Parameters::PressureMin2()
{
   return mPressureMin2;
}
bool     Parameters::PressureNom2( QString const& val )
{
   return ParseValue( mPressureNom2, val );
}
double   Parameters::PressureNom2()
{
   return mPressureNom2;
}
bool     Parameters::PressureMax2( QString const& val )
{
   return ParseValue( mPressureMax2, val );
}
double   Parameters::PressureMax2()
{
   return mPressureMax2;
}


bool     Parameters::FrequencyMin( QString const& val )
{
   return ParseValue( mFrequencyMin, val );
}
qint32   Parameters::FrequencyMin()
{
   return mFrequencyMin;
}
bool     Parameters::FrequencyNom( QString const& val )
{
   return ParseValue( mFrequencyNom, val );
}
qint32   Parameters::FrequencyNom()
{
   return mFrequencyNom;
}
bool     Parameters::FrequencyMax( QString const& val )
{
   return ParseValue( mFrequencyMax, val );
}
qint32   Parameters::FrequencyMax()
{
   return mFrequencyMax;
}

bool     Parameters::VolumeKPD( QString const& val )
{
   return ParseValue( mVolumeKPD, val );
}
double   Parameters::VolumeKPD()
{
   return mVolumeKPD;
}
bool     Parameters::Expenditure( QString const& val )
{
   return ParseValue( mExpenditure, val );
}
double   Parameters::Expenditure()
{
   return mExpenditure;
}

}//namespace pump
}//namespace test
