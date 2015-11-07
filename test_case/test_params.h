#pragma once
#include <QString>
#include "devices/device.h"
#include "test_case.h"
#include <QPainter>

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
    DN_UP = 0,
    DN_DOWN = 1
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
    ST_100_mA = 5,
    ST_300_mA = 6,
    ST_600_mA = 7,
    ST_860_mA = 8,
};

bool ParseValue ( CONTROL_SIGNAL& sig, QString const& val );
bool ParseValue ( CONTROL_TYPE& sig, QString const& val );
bool ParseValue ( VOLTAGE_TYPE& sig, QString const& val );
bool ParseValue ( qint32& param, QString const& val );
bool ParseValue ( DYNAMIC& param, QString const& val );
bool ParseValue ( RELL_CONTROL& param, QString const& val );
bool ParseValue ( double &param, QString const& val );
bool ParseValue ( SIGNAL_TYPE &param, QString const& val );

QString ToString( qint32 const& v );
QString ToString( bool const& v );
QString ToString( VOLTAGE_TYPE const& v );
QString ToString( CONTROL_TYPE const& v );
QString ToString( CONTROL_SIGNAL const& v );
QString ToString( DYNAMIC const& v );
QString ToString( RELL_CONTROL const& v );
QString ToString( double const& v );
QString ToString( SIGNAL_TYPE const& v );

class Parameters
{
public:
    typedef QList< test::Test* > TestsList;
    Parameters();
    virtual ~Parameters(){}
    virtual QString ToString() const = 0;

    virtual test::TestCase& TestCollection() const = 0;

    virtual QJsonObject Serialise() const;
    virtual bool Deserialize( QJsonObject const& obj );

    virtual void WriteToController() const = 0;

    virtual bool Draw(QPainter &painter, QRect &free_rect ) const;
    virtual bool DrawResults(QPainter &/*painter*/, QRect &/*free_rect*/ ) const
    { return true; }
    void Reset();

    void TestCase ( TestsList const& test_case);
    TestsList const& TestCase () const;

protected:
    TestsList mTestCase;        //набор тестов для выполнения
};

class CommonParameters :public Parameters
{
public:
    CommonParameters();
    virtual ~CommonParameters(){}
    virtual QString ToString() const = 0;

    virtual test::TestCase& TestCollection() const = 0;

    virtual QJsonObject Serialise() const;
    virtual bool Deserialize( QJsonObject const& obj );

    void Reset();

    bool SerNo ( QString const& val );
    QString const& SerNo () const;

    bool ReelCount ( QString const& val );
    qint32 const& ReelCount () const;

    bool MaxExpenditure ( QString const& val );
    const double &MaxExpenditure() const;

    bool ControlType ( QString const& val );
    CONTROL_TYPE const& ControlType () const;

    bool MinControlPressure ( QString const& val );
    const double &MinControlPressure() const;

    bool MaxControlPressure ( QString const& val );
    const double &MaxControlPressure() const;

protected:
    QString mSerNo;             //Серийный номер
    qint32 mReelCount;          //+Количество катушек питания

    double mMaxExpenditure;     //+максимальный расход

    CONTROL_TYPE mControlType;  //+Тип управления гидрораспределителем
    double mMinControlPressure; //+минимальное давление управления
    double mMaxControlPressure; //+максимальное давление управления

private:
    size_t mPrintedPage;
};

extern Parameters* CURRENT_PARAMS;

void ParamsToFile( QString fname, Parameters const& params );
Parameters* ParamsFromFile( QString fname );

void DataToFile( QString fname, Parameters const& params );
bool DataFromFile( QString fname );

}//namespace test
