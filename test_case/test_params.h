#pragma once
#include <QString>
#include "devices/device.h"
#include "test_case.h"

namespace test
{

class Test;

enum CONTROL_SIGNAL
{
    CS_UNKNOWN = -1,
    CS_NONE = 0,
    CS_REEL_A = 1,
    CS_REEL_B = 2,

};
enum CONTROL_TYPE
{
    CT_UNKNOWN = -1,
    CT_ELECTRIC,
    CT_HYDRO_ELECTRIC
};
enum VOLTAGE_TYPE
{
    VT_UNKNOWN = -1,
    VT_AC,  //переменное
    VT_DC   //постоянное
};
enum DYNAMIC
{
    DN_UNKNOWN = -1,
    DN_UP,
    DN_DOWN
};
enum DYNAMIC_CONTROL
{
    DC_UNKNOWN = -1,
    DC_DD1,
    DC_DD2,
    DC_DD3
};
enum RELL_CONTROL
{
    RC_UNKNOWN = -1,
    RC_REEL,
    RC_CONTROL_BOX
};
enum SIGNAL_TYPE
{
    ST_UNKNOWN = -1,
    ST_4_20_mA = 0,
    ST_0_20_mA = 1,
    ST_10_10_V = 2,
    ST_10_10_mA = 3,
    ST_40_40_mA = 4,

};

bool ParseValue ( CONTROL_SIGNAL& sig, QString const& val );
bool ParseValue ( CONTROL_TYPE& sig, QString const& val );
bool ParseValue ( VOLTAGE_TYPE& sig, QString const& val );
bool ParseValue ( qint32& param, QString const& val );
bool ParseValue ( DYNAMIC& param, QString const& val );
bool ParseValue ( DYNAMIC_CONTROL& param, QString const& val );
bool ParseValue ( RELL_CONTROL& param, QString const& val );
bool ParseValue (double &param, QString const& val );
bool ParseValue (SIGNAL_TYPE &param, QString const& val );

QString ToString( qint32 const& v );
QString ToString( VOLTAGE_TYPE const& v );
QString ToString( CONTROL_TYPE const& v );
QString ToString( CONTROL_SIGNAL const& v );
QString ToString( DYNAMIC const& v );
QString ToString( DYNAMIC_CONTROL const& v );
QString ToString( RELL_CONTROL const& v );
QString ToString( double const& v );
QString ToString( SIGNAL_TYPE const& v );

class Parameters
{
public:
    typedef QList< test::Test* > TestsList;
    Parameters();
    virtual ~Parameters(){}
    virtual QString ToString() = 0;

    virtual test::TestCase& TestCollection() const = 0;

    virtual QJsonObject Serialise() const;
    virtual bool Deserialize( QJsonObject const& obj );

    void Reset();

    void TestCase ( TestsList const& test_case);
    TestsList const& TestCase ();

protected:
    TestsList mTestCase;        //набор тестов для выполнения
};

class CommonParameters :public Parameters
{
public:
    CommonParameters();
    virtual ~CommonParameters(){}
    virtual QString ToString() = 0;

    virtual test::TestCase& TestCollection() const = 0;

    virtual QJsonObject Serialise() const;
    virtual bool Deserialize( QJsonObject const& obj );

    void Reset();

    bool SerNo ( QString const& val );
    QString const& SerNo () const;

    bool ReelCount ( QString const& val );
    qint32 const& ReelCount () const;

    bool MaxExpenditure ( QString const& val );
    qint32 const& MaxExpenditure () const;

    bool ControlType ( QString const& val );
    CONTROL_TYPE const& ControlType () const;

    bool MinControlPressure ( QString const& val );
    qint32 const& MinControlPressure () const;

    bool MaxControlPressure ( QString const& val );
    qint32 const& MaxControlPressure () const;

protected:
    QString mSerNo;             //Серийный номер
    qint32 mReelCount;          //Количество катушек питания

    qint32 mMaxExpenditure;     //максимальный расход

    CONTROL_TYPE mControlType;  //Тип управления гидрораспределителем
    qint32 mMinControlPressure; //минимальное давление управления
    qint32 mMaxControlPressure; //максимальное давление управления
};

extern Parameters* CURRENT_PARAMS;

void ToFile( QString fname, Parameters const& params );
Parameters* FromFile( QString fname );

}//namespace test
