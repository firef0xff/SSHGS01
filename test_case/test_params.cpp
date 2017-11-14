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
#include "test_case/implementation/test_params_pumps.h"
#include <QTextDocument>

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
bool ParseValue ( RELL_CONTROL& param, QString const& val )
{
    if ( !val.compare( "Катушками управления", Qt::CaseInsensitive ) )
    {
        param = RC_REEL;
        return true;
    }
    else if ( !val.compare( "Блок управления", Qt::CaseInsensitive ) )
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
    else if( !val.compare( "+-15 мА", Qt::CaseInsensitive ) )
    {
        param = ST_15_15_mA;
        return true;
    }
    else if( !val.compare( "+-20 мА", Qt::CaseInsensitive ) )
    {
        param = ST_20_20_mA;
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
    else if ( !val.compare( "0..10 В", Qt::CaseInsensitive ) )
    {
        param = ST_0_10_V;
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
    else if( !val.compare( "1600 мА", Qt::CaseInsensitive ) )
    {
        param = ST_1600_mA;
        return true;
    }
    else if( !val.compare( "2500 мА", Qt::CaseInsensitive ) )
    {
        param = ST_2500_mA;
        return true;
    }
    else if( !val.compare( "3750 мА", Qt::CaseInsensitive ) )
    {
        param = ST_3750_mA;
        return true;
    }
    else if( !val.compare( "5000 мА", Qt::CaseInsensitive ) )
    {
        param = ST_5000_mA;
        return true;
    }
    return false;
}

QString ToString( qint32 const& v )
{   
    return QString::number( v );
}
QString ToString( bool const& v )
{
    if ( v )
        return QString( "Да" );
    return QString( "Нет" );
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
QString ToString( RELL_CONTROL const& v )
{
    switch (v)
    {
        case RC_UNKNOWN:
            return "Не задано";
        case RC_REEL:
            return "Катушками управления";
        case RC_CONTROL_BOX:
            return "Блок управления";
    default:
        return "Неизвестное значение";
    }
}
QString ToShortString( RELL_CONTROL const& v )
{
    switch (v)
    {
        case RC_UNKNOWN:
            return "Не задано";
        case RC_REEL:
            return "Кат. упр.";
        case RC_CONTROL_BOX:
            return "Блок упр.";
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
        case ST_15_15_mA:
            return "+-15 мА";
        case ST_20_20_mA:
            return "+-20 мА";
        case ST_40_40_mA:
            return "+-40 мА";
        case ST_10_10_V:
            return "+-10 В";
        case ST_0_10_V:
            return "0..10 В";
        case ST_100_mA:
            return "100 мА";
        case ST_300_mA:
            return "300 мА";
        case ST_600_mA:
            return "600 мА";
        case ST_860_mA:
            return "860 мА";
        case ST_1600_mA:
            return "1600 мА";
        case ST_2500_mA:
            return "2500 мА";
        case ST_3750_mA:
            return "3750 мА";
        case ST_5000_mA:
            return "5000 мА";
    default:
        return "Неизвестное значение";
    }
}

Parameters::Parameters():
    mTestCase(),
    mDefaultExpenditure( 0.0 )
{}
void Parameters::Reset()
{
    mTestCase.clear();
    mUser.clear();
    mDate = QDateTime();
    mDefaultExpenditure = 0.0;
}

void Parameters::TestCase ( TestsList const& test_case)
{
    mTestCase = test_case;
}
Parameters::TestsList const& Parameters::TestCase () const
{
    return mTestCase;
}

void Parameters::Date( QDateTime const& data )
{
    mDate = data;
}
QDateTime const& Parameters::Date()
{
    return mDate;
}

void Parameters::User( QString const& user )
{
    mUser = user;
}
QString const& Parameters::User()
{
    return mUser;
}

void Parameters::ReportType( QString const& val )
{
    mReportType = val;
}
QString const& Parameters::ReportType()
{
    return mReportType;
}

bool Parameters::DefaultExpenditure( QString const& value )
{
    return ParseValue( mDefaultExpenditure, value );
}
const double &Parameters::DefaultExpenditure() const
{
    return mDefaultExpenditure;
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
    obj.insert("Date", mDate.toString(Qt::ISODate));
    obj.insert("User", mUser);
    obj.insert("ReportType", mReportType);
    obj.insert("DefaultExpenditure", mDefaultExpenditure);
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

    mDate = QDateTime::fromString( obj.value("Date").toString(), Qt::ISODate );
    mUser = obj.value("User").toString();
    mReportType = obj.value("ReportType").toString();
    mDefaultExpenditure = obj.value("DefaultExpenditure").toDouble();

    return res;
}

void Parameters::StendInit() const
{
    WriteToController();
}
void Parameters::StendDeInit() const
{}

bool Parameters::Draw(QPainter &painter, QRect &free_rect, QString const& compare_width ) const
{
    QFont text_font = painter.font();
    text_font.setPointSize( 12 );
    painter.setFont( text_font );

    QRect r( 0, 0, free_rect.width(), free_rect.height() );
    painter.save();
    painter.translate( free_rect.topLeft() );
    QTextDocument doc;
    doc.setUndoRedoEnabled( false );
    doc.setTextWidth( free_rect.width() );
    doc.setUseDesignMetrics( true );
    doc.setDefaultTextOption ( QTextOption (Qt::AlignLeft )  );
    doc.setPlainText( ToString() );

    doc.drawContents( &painter, r );

    painter.restore();
    return true;
}

QString Parameters::ModelId() const
{
    return "";
}

CommonParameters::CommonParameters():
    mSerNo(""),
    mPosCount( 0 ),
    mMaxExpenditure ( 0.0 ),
    mControlType ( CT_UNKNOWN ),
    mMinControlPressure ( 0.0 ),
    mMaxControlPressure ( 0.0 )
{}

void CommonParameters::Reset()
{
    mSerNo = "";
    mPosCount =  0;
    mMaxExpenditure  =  0.0;
    mControlType  =  CT_UNKNOWN;
    mMinControlPressure  =  0.0;
    mMaxControlPressure  =  0.0;
    Parameters::Reset();
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

bool CommonParameters::PosCount ( QString const& val )
{
    return ParseValue( mPosCount, val );
}
qint32 const& CommonParameters::PosCount () const
{
    return mPosCount;
}

bool CommonParameters::MaxExpenditure ( QString const& val )
{
    return ParseValue( mMaxExpenditure, val );
}
double const& CommonParameters::MaxExpenditure () const
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
double const& CommonParameters::MinControlPressure () const
{
    return mMinControlPressure;
}

bool CommonParameters::MaxControlPressure ( QString const& val )
{
    return ParseValue( mMaxControlPressure, val );
}
double const& CommonParameters::MaxControlPressure () const
{
    return mMaxControlPressure;
}

QJsonObject CommonParameters::Serialise() const
{    
    QJsonObject obj = Parameters::Serialise();
    obj.insert("SerNo", mSerNo);
    obj.insert("PosCount", mPosCount);
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
    mPosCount = obj.value("PosCount").toInt();
    mMaxExpenditure = obj.value("MaxExpenditure").toDouble();
    mControlType = static_cast<CONTROL_TYPE>( obj.value("ControlType").toInt() );
    mMinControlPressure = obj.value("MinControlPressure").toDouble();
    mMaxControlPressure = obj.value("MaxControlPressure").toDouble();

    return res;
}

QString CommonParameters::ModelId() const
{
    return mSerNo;
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
        else if ( pump::Parameters::Instance().Deserialize( obj ) )
            ret = &pump::Parameters::Instance();
        f.close();
    }

    return ret;
}

void DataToFile( QString fname, Parameters const& params )
{
    QFile f( fname );
    f.open(QIODevice::WriteOnly);
    QJsonDocument doc;
    doc.setObject( GetTestData( params ) );
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
        else if ( pump::Parameters::Instance().Deserialize( params ) )
            ret = &pump::Parameters::Instance();

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

    if ( ret )
        test::CURRENT_PARAMS = ret;
    return ret != nullptr;
}

QJsonObject GetTestData( Parameters const& params )
{
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
    return data;
}

void SaveToEtalone( const Parameters &params )
{
    auto obj = ReadFromEtalone();
    obj.insert( params.ModelId(), GetTestData(params) );
    QFile f( "models.json" );
    f.open(QIODevice::WriteOnly);
    QJsonDocument doc;
    doc.setObject( obj );
    f.write( doc.toJson() );

    f.close();
}

QJsonObject ReadFromFile( QString const& f_name )
{
    QFile f( f_name );
    if ( f.exists() )
    {
        f.open(QIODevice::ReadOnly);
        auto doc = QJsonDocument::fromJson( f.readAll() );
        f.close();

        return doc.object();
    }

    return QJsonObject();
}

QJsonObject ReadFromEtalone()
{    
    return ReadFromFile( "models.json" );
}

}//namespace test

