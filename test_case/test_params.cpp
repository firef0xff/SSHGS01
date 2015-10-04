#include "test_params.h"
#include "tests.h"
#include <memory>
#include <mutex>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QLocale>
#include "test_case/implementation/test_params_hydro.h"
#include "test_case/implementation/test_params_servo.h"
#include "test_case/implementation/test_params_control_panel.h"
#include "test_case/implementation/test_params_hydro_cilinder.h"

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
bool ParseValue ( double& param, QString const& val )
{
    QChar const decimalsep = QLocale::system().decimalPoint();
    bool t = false;
    auto b =  QString(val).replace( decimalsep, "." ).toDouble( &t );

    if ( t )
        param = b;
    return t;
}
bool ParseValue (SIGNAL_TYPE &param, QString const& val )
{
    if ( !val.compare( "0..20 мА", Qt::CaseInsensitive ) )
    {
        param = ST_0_20_mA;
        return true;
    }
    else if ( !val.compare( "4..20 мА", Qt::CaseInsensitive ) )
    {
        param = ST_4_20_mA;
        return true;
    }
    else if ( !val.compare( "+-10 мА", Qt::CaseInsensitive ) )
    {
        param = ST_10_10_mA;
        return true;
    }
    else if ( !val.compare( "+-40 мА", Qt::CaseInsensitive ) )
    {
        param = ST_40_40_mA;
        return true;
    }
    else if ( !val.compare( "+-10 В", Qt::CaseInsensitive ) )
    {
        param = ST_10_10_V;
        return true;
    }
    else if( !val.compare( "100 мА", Qt::CaseInsensitive ) )
    {
        param = ST_100_mA;
        return true;
    }
    else if( !val.compare( "300 мА", Qt::CaseInsensitive ) )
    {
        param = ST_300_mA;
        return true;
    }
    else if( !val.compare( "600 мА", Qt::CaseInsensitive ) )
    {
        param = ST_600_mA;
        return true;
    }
    else if( !val.compare( "860 мА", Qt::CaseInsensitive ) )
    {
        param = ST_860_mA;
        return true;
    }
    return false;
}

QString ToString( qint32 const& v )
{   
    return QString::number( v );
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
QString ToString( double const& v )
{
    QChar const decimalsep = QLocale::system().decimalPoint();
    return QString::number( v ).replace( ".", decimalsep );
}
QString ToString( SIGNAL_TYPE const& v )
{
    switch (v)
    {
        case ST_UNKNOWN:
            return "Не задано";
        case ST_0_20_mA:
            return "0..20 мА";
        case ST_4_20_mA:
            return "4..20 мА";
        case ST_10_10_mA:
            return "+-10 мА";
        case ST_40_40_mA:
            return "+-40 мА";
        case ST_10_10_V:
            return "+-10 В";
        case ST_100_mA:
            return "100 мА";
        case ST_300_mA:
            return "300 мА";
        case ST_600_mA:
            return "600 мА";
        case ST_860_mA:
            return "860 мА";
    default:
        return "Неизвестное значение";
    }
}

Parameters::Parameters():
    mTestCase()
{}
void Parameters::Reset()
{
    mTestCase.clear();
}

void Parameters::TestCase ( TestsList const& test_case)
{
    mTestCase = test_case;
}
Parameters::TestsList const& Parameters::TestCase () const
{
    return mTestCase;
}

QJsonObject Parameters::Serialise() const
{
    QJsonObject obj;
    QJsonArray tests;
    foreach (test::Test* ptr, mTestCase)
    {
        tests.push_back( ptr->ID() );
    }
    obj.insert("TestCase", tests);
    return obj;
}
bool Parameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool res = true;
    QJsonArray tests = obj.value("TestCase").toArray();

    test::TestCase const& Collection = TestCollection();
    foreach ( QJsonValue const& val, tests )
    {
        bool find = false;
        for ( auto it = Collection.Tests().cbegin(), end = Collection.Tests().cend();
              it != end && !find ; ++it)
        {
            test::Test* ptr = *it;
            if ( ptr->ID() == val.toInt() )
            {
                mTestCase.push_back( ptr );
                find = true;
            }
        }
        res *= find;
    }

    return res;
}


CommonParameters::CommonParameters():
    mSerNo(""),
    mReelCount( 0 ),
    mMaxExpenditure ( 0 ),
    mControlType ( CT_UNKNOWN ),
    mMinControlPressure ( 0 ),
    mMaxControlPressure ( 0 )
{}

void CommonParameters::Reset()
{
    mSerNo = "";
    mReelCount =  0;
    mMaxExpenditure  =  0;
    mControlType  =  CT_UNKNOWN;
    mMinControlPressure  =  0;
    mMaxControlPressure  =  0;
}

bool CommonParameters::SerNo ( QString const& val )
{
    mSerNo = val;
    return true;
}
QString const& CommonParameters::SerNo () const
{
    return mSerNo;
}

bool CommonParameters::ReelCount ( QString const& val )
{
    return ParseValue( mReelCount, val );
}
qint32 const& CommonParameters::ReelCount () const
{
    return mReelCount;
}

bool CommonParameters::MaxExpenditure ( QString const& val )
{
    return ParseValue( mMaxExpenditure, val );
}
qint32 const& CommonParameters::MaxExpenditure () const
{
    return mMaxExpenditure;
}

bool CommonParameters::ControlType ( QString const& val )
{
    return ParseValue( mControlType, val );
}
CONTROL_TYPE const& CommonParameters::ControlType () const
{
    return mControlType;
}

bool CommonParameters::MinControlPressure ( QString const& val )
{
    return ParseValue( mMinControlPressure, val );
}
qint32 const& CommonParameters::MinControlPressure () const
{
    return mMinControlPressure;
}

bool CommonParameters::MaxControlPressure ( QString const& val )
{
    return ParseValue( mMaxControlPressure, val );
}
qint32 const& CommonParameters::MaxControlPressure () const
{
    return mMaxControlPressure;
}

QJsonObject CommonParameters::Serialise() const
{    
    QJsonObject obj = Parameters::Serialise();
    obj.insert("SerNo", mSerNo);
    obj.insert("ReelCount", mReelCount);
    obj.insert("MaxExpenditure", mMaxExpenditure);
    obj.insert("ControlType",mControlType);
    obj.insert("MinControlPressure",mMinControlPressure);
    obj.insert("MaxControlPressure",mMaxControlPressure);

    return obj;
}
bool CommonParameters::Deserialize(const QJsonObject &obj )
{
    Reset();
    bool res = Parameters::Deserialize( obj );
    mSerNo = obj.value("SerNo").toString();
    mReelCount = obj.value("ReelCount").toInt();
    mMaxExpenditure = obj.value("MaxExpenditure").toInt();
    mControlType = static_cast<CONTROL_TYPE>( obj.value("ControlType").toInt() );
    mMinControlPressure = obj.value("MinControlPressure").toInt();
    mMaxControlPressure = obj.value("MaxControlPressure").toInt();

    return res;
}


void ParamsToFile( QString fname, Parameters const& params )
{
    QFile f( fname );
    f.open(QIODevice::WriteOnly);
    QJsonDocument doc;
    doc.setObject( params.Serialise() );

    f.write( doc.toJson() );

    f.close();
}
Parameters* ParamsFromFile( QString fname )
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
        else if ( servo::Parameters::Instance().Deserialize( obj ) )
            ret = &servo::Parameters::Instance();
        else if ( hydro_cylinder::Parameters::Instance().Deserialize( obj ) )
            ret = &hydro_cylinder::Parameters::Instance();
        else if ( control_board::Parameters::Instance().Deserialize( obj ) )
            ret = &control_board::Parameters::Instance();
        f.close();
    }

    return ret;
}

void DataToFile( QString fname, Parameters const& params )
{
    QFile f( fname );
    f.open(QIODevice::WriteOnly);
    QJsonDocument doc;

    QJsonObject data;
    data.insert( "Params", params.Serialise() );

    QJsonArray tests_data;
    foreach ( Test *d, params.TestCase() )
    {
        QJsonObject obj;
        obj.insert("id", d->ID() );
        obj.insert("data", d->Serialise() );

        tests_data.push_back( obj );
    }
    data.insert( "Results", tests_data );
    doc.setObject( data );
    f.write( doc.toJson() );

    f.close();
}

bool DataFromFile( QString fname )
{
    QFile f( fname );
    Parameters* ret = nullptr;
    if ( f.exists() )
    {
        f.open(QIODevice::ReadOnly);
        auto doc = QJsonDocument::fromJson( f.readAll() );
        QJsonObject params = doc.object().value("Params").toObject();
        QJsonArray tests_data = doc.object().value("Results").toArray();

        if ( hydro::Parameters::Instance().Deserialize( params ) )
            ret = &hydro::Parameters::Instance();
        else if ( servo::Parameters::Instance().Deserialize( params ) )
            ret = &servo::Parameters::Instance();
        else if ( hydro_cylinder::Parameters::Instance().Deserialize( params ) )
            ret = &hydro_cylinder::Parameters::Instance();
        else if ( control_board::Parameters::Instance().Deserialize( params ) )
            ret = &control_board::Parameters::Instance();

        if ( ret )
        {
            foreach (QJsonValue const& val, tests_data)
            {
                QJsonObject obj = val.toObject();
                uint8_t id = obj.value("id").toInt();
                QJsonObject data = obj.value("data").toObject();

                auto it = std::find_if( ret->TestCase().begin(), ret->TestCase().end(),
                [ &id ]( Test* ptr )
                {
                    return ptr->ID() == id;
                } );

                if ( it != ret->TestCase().end() )
                {
                    Test* ptr = *it;
                    ptr->Deserialize( data );
                }
            }
        }
        f.close();
    }

    return ret != nullptr;
}

}//namespace test

