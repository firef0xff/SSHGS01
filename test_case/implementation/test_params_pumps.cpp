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
   //Перед началом испытания необходимо задать следующие параметры:
   mSerianNo = "";   //Серийный номер //Идентификационный номер
   mModel = ""; //Модель гидронасоса
   mSectionsCount = 0; //Количество секций в насосе
   mSpin = spUnknown;   //Направление вращения

   mElektrikControl = false; //Наличие электронного управления
   mVoltage = 0; //Напряжение питания, В
   mVoltageType = VT_UNKNOWN; //Тип тока

   mHydroControl = false; //Наличие гидравлического управления
   mMaxControlPressure = 0; //Максимальное давление в канале управления, бар

   mWorkVolume1 = 0.0; //Рабочий объем насоса №1

  //Давление в напорной полости насоса №1:
   mPressureMin1 = 0.0; //Минимальное
   mPressureNom1 = 0.0; //номинальное (рабочее)
   mPressureMax1 = 0.0; //максимальное

   mWorkVolume2 = 0.0; //Рабочий объем насоса №2

  //Давление в напорной полости насоса №2
   mPressureMin2 = 0.0; //Минимальное
   mPressureNom2 = 0.0; //номинальное (рабочее)
   mPressureMax2 = 0.0; //максимальное

  //Частота вращения насоса(ов):
   mFrequencyMin = 0; //Минимальная
   mFrequencyNom = 0; //Номинальная
   mFrequencyMax = 0; //Максимальная

   mVolumeKPD = 0.0; //Объемный кпд насоса
   mExpenditure = 0.0; //Дренаж насоса при номинальном давлении и номинальном расходе л/мин

  //Сервисные настройки для испытания насосов:
   mFuncTestTime = 0; //Продолжительность испытаний функционирования
   mStrongTestTime = 0; //Продолжительность испытаний на прочность
   mHermTestTime = 0; //Продолжительность испытаний наружной герметичности

   mX = 0;//Время набора требуемого давления, сек (X)
   mY = 0;//Время набора требуемой частоты вращения, сек (Y)
   mZ = 0;//Время стабилизации потока, сек (Z)
   mA = 0.0;//Коэффициент функционирования, % (А)
   mA1 = 0.0;//Коэффициент функционирования для дренажа, % (А1)
   mE = 0.0;//Точность для испытаний функциональные зависимости, сек (Е)
   mB = 0;//Продолжительность испытаний функциональные зависимости, сек (В)
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
    obj.insert("mSerianNo", mSerianNo);
    obj.insert("mModel", mModel);
    obj.insert("mSectionsCount", mSectionsCount);
    obj.insert("mSpin", mSpin);

    obj.insert("mElektrikControl", mElektrikControl);
    obj.insert("mVoltage", mVoltage);
    obj.insert("mVoltageType", mVoltageType);

    obj.insert("mHydroControl", mHydroControl);
    obj.insert("mMaxControlPressure", mMaxControlPressure);

    obj.insert("mWorkVolume1", mWorkVolume1);
    obj.insert("mPressureMin1", mPressureMin1);
    obj.insert("mPressureNom1", mPressureNom1);
    obj.insert("mPressureMax1", mPressureMax1);

    obj.insert("mWorkVolume2", mWorkVolume2);
    obj.insert("mPressureMin2", mPressureMin2);
    obj.insert("mPressureNom2", mPressureNom2);
    obj.insert("mPressureMax2", mPressureMax2);

    obj.insert("mFrequencyMin", mFrequencyMin);
    obj.insert("mFrequencyNom", mFrequencyNom);
    obj.insert("mFrequencyMax", mFrequencyMax);

    obj.insert("mVolumeKPD", mVolumeKPD);
    obj.insert("mExpenditure", mExpenditure);

    //Сервисные настройки для испытания насосов:
    obj.insert("mFuncTestTime", mFuncTestTime);
    obj.insert("mStrongTestTime", mStrongTestTime);
    obj.insert("mHermTestTime", mHermTestTime);

    obj.insert("mX", mX);
    obj.insert("mY", mY);
    obj.insert("mZ", mZ);
    obj.insert("mA", mA);
    obj.insert("mA1", mA1);
    obj.insert("mE", mE);
    obj.insert("mB", mB);

    QJsonObject ret = test::Parameters::Serialise();
    ret.insert("pump", obj);

    return ret;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool ret = test::Parameters::Deserialize( obj );
    auto val = obj.value("pump");
    if ( val.isObject() )
    {
        QJsonObject obj = val.toObject();
        mSerianNo = obj.value("mSerianNo").toString();
        mModel = obj.value("mModel").toString();
        mSectionsCount = obj.value("mSectionsCount").toInt();
        mSpin = static_cast<SPIN>( obj.value("mSpin").toInt() );

        mElektrikControl = obj.value("mElektrikControl").toBool();
        mVoltage = obj.value("mVoltage").toDouble();
        mVoltageType = static_cast<VOLTAGE_TYPE>( obj.value("mVoltageType").toInt() );

        mHydroControl = obj.value("mHydroControl").toBool();
        mMaxControlPressure = obj.value("mMaxControlPressure").toDouble();

        mWorkVolume1 = obj.value("mWorkVolume1").toDouble();
        mPressureMin1 = obj.value("mPressureMin1").toDouble();
        mPressureNom1 = obj.value("mPressureNom1").toDouble();
        mPressureMax1 = obj.value("mPressureMax1").toDouble();

        mWorkVolume2 = obj.value("mWorkVolume2").toDouble();
        mPressureMin2 = obj.value("mPressureMin2").toDouble();
        mPressureNom2 = obj.value("mPressureNom2").toDouble();
        mPressureMax2 = obj.value("mPressureMax2").toDouble();

        mFrequencyMin = obj.value("mFrequencyMin").toInt();
        mFrequencyNom = obj.value("mFrequencyNom").toInt();
        mFrequencyMax = obj.value("mFrequencyMax").toInt();

        mVolumeKPD = obj.value("mVolumeKPD").toDouble();
        mExpenditure = obj.value("mExpenditure").toDouble();

        //Сервисные настройки для испытания насосов:
        mFuncTestTime = obj.value("mFuncTestTime").toInt();
        mStrongTestTime = obj.value("mStrongTestTime").toInt();
        mHermTestTime = obj.value("mHermTestTime").toInt();

        mX = obj.value("mX").toInt();
        mY = obj.value("mY").toInt();
        mZ = obj.value("mZ").toInt();
        mA = obj.value("mA").toDouble();
        mA1 = obj.value("mA1").toDouble();
        mE = obj.value("mE").toDouble();
        mB = obj.value("mB").toInt();
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
    return mModel;
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

//параметры испытания
bool Parameters::FuncTestTime( QString const& val )
{
   return ParseValue( mFuncTestTime, val );
}
qint32 Parameters::FuncTestTime()
{
   return mFuncTestTime;
}
bool Parameters::StrongTestTime( QString const& val )
{
   return ParseValue( mStrongTestTime, val );
}
qint32 Parameters::StrongTestTime()
{
   return mStrongTestTime;
}
bool Parameters::HermTestTime( QString const& val )
{
   return ParseValue( mHermTestTime, val );
}
qint32 Parameters::HermTestTime()
{
   return mHermTestTime;
}

bool Parameters::X( QString const& val )
{
   return ParseValue( mX, val );
}
qint32 Parameters::X()
{
   return mX;
}
bool Parameters::Y( QString const& val )
{
   return ParseValue( mY, val );
}
qint32 Parameters::Y()
{
   return mY;
}
bool Parameters::Z( QString const& val )
{
   return ParseValue( mZ, val );
}
qint32 Parameters::Z()
{
   return mZ;
}
bool Parameters::A( QString const& val )
{
   return ParseValue( mA, val );
}
double Parameters::A()
{
   return mA;
}
bool Parameters::A1( QString const& val )
{
   return ParseValue( mA1, val );
}
double Parameters::A1()
{
   return mA1;
}
bool Parameters::E( QString const& val )
{
   return ParseValue( mE, val );
}
double Parameters::E()
{
   return mE;
}
bool Parameters::B( QString const& val )
{
   return ParseValue( mB, val );
}
qint32 Parameters::B()
{
   return mB;
}

}//namespace pump
}//namespace test
