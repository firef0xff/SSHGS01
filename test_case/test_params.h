#pragma once
#include <QString>
#include "devices/device.h"

namespace test
{

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


bool ParseValue ( CONTROL_SIGNAL& sig, QString const& val );
bool ParseValue ( CONTROL_TYPE& sig, QString const& val );
bool ParseValue ( VOLTAGE_TYPE& sig, QString const& val );
bool ParseValue ( qint32& param, QString const& val );
bool ParseValue ( DYNAMIC& param, QString const& val );
bool ParseValue ( DYNAMIC_CONTROL& param, QString const& val );
bool ParseValue ( RELL_CONTROL& param, QString const& val );
bool ParseValue ( double_t& param, QString const& val );

class Parameters
{
public:
    Parameters();
    virtual ~Parameters(){}
    virtual QString ToString() = 0;

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

namespace hydro
{

class Parameters : public test::Parameters
{
public:
    static Parameters& Instance();

    void Reset();
    QString ToString();

    bool GsType ( QString const& val );
    QString const& GsType () const;

    bool Voltage ( QString const& val );
    qint32 const& Voltage () const;

    bool VoltageRange ( QString const& val );
    qint32 const& VoltageRange () const;

    bool VoltageType ( QString const& val );
    const VOLTAGE_TYPE &VoltageType() const;

    bool MaxWorkPressure ( QString const& val );
    qint32 const& MaxWorkPressure () const;

    bool MinTestPressure ( QString const& val );
    qint32 const& MinTestPressure () const;

    bool HermPressure ( QString const& val );
    qint32 const& HermPressure () const;

    bool HermSignal ( QString const& val );
    CONTROL_SIGNAL const& HermSignal () const;

    bool PABTSignal ( QString const& val );
    CONTROL_SIGNAL const& PABTSignal () const;

    bool PBATSignal ( QString const& val );
    CONTROL_SIGNAL const& PBATSignal () const;

    bool ActuationOnTime ( QString const& val );
    qint32 const& ActuationOnTime () const;

    bool ActuationOffTime ( QString const& val );
    qint32 const& ActuationOffTime () const;

    bool OnControl_1 ( QString const& val );
    DYNAMIC_CONTROL const& OnControl_1 () const;
    bool OffControl_1 ( QString const& val );
    DYNAMIC_CONTROL const& OffControl_1 () const;
    bool OnControl_2 ( QString const& val );
    DYNAMIC_CONTROL const& OnControl_2 () const;
    bool OffControl_2 ( QString const& val );
    DYNAMIC_CONTROL const& OffControl_2 () const;

    bool OnDynamic_1 ( QString const& val );
    DYNAMIC const& OnDynamic_1 () const;
    bool OffDynamic_1 ( QString const& val );
    DYNAMIC const& OffDynamic_1 () const;
    bool OnDynamic_2 ( QString const& val );
    DYNAMIC const& OnDynamic_2 () const;
    bool OffDynamic_2 ( QString const& val );
    DYNAMIC const& OffDynamic_2 () const;

private:
    Parameters();
    Parameters( Parameters const& ) = delete;
    void operator = ( Parameters& ) = delete;

    QString mGsType;            //тип гидрораспределителя
    qint32 mVoltage;            //Напряжение питания
    qint32 mVoltageRange;       //Допустимое отклонение напрядения питания
    VOLTAGE_TYPE mVoltageType;  //тип напряжения    
    qint32 mMaxWorkPressure;    //максимальное рабочее давление

    qint32 mMinTestPressure;    //давление для испытания функционирования минимальным давлением
    qint32 mHermPressure;       //давление для испытания функционирования минимальным давлением

    CONTROL_SIGNAL mHermSignal; //сигнал проверки внутренней герметичности
    CONTROL_SIGNAL mPABTSignal; //сигнал PABT
    CONTROL_SIGNAL mPBATSignal; //сигнал PBAT

    qint32 mActuationOnTime;    //время срабатывания включения распределителя
    qint32 mActuationOffTime;   //время срабатывания выключения распределителя

    DYNAMIC_CONTROL mOnControl_1; //датчик контроля включения 1
    DYNAMIC_CONTROL mOffControl_1;//датчик контроля выключения 1
    DYNAMIC_CONTROL mOnControl_2; //датчик контроля включения 2
    DYNAMIC_CONTROL mOffControl_2;//датчик контроля выключения 2

    DYNAMIC mOnDynamic_1;    //ожидаемая динамика на датчике включения 1
    DYNAMIC mOffDynamic_1;   //ожидаемая динамика на датчике выключения 1
    DYNAMIC mOnDynamic_2;    //ожидаемая динамика на датчике включения 2
    DYNAMIC mOffDynamic_2;   //ожидаемая динамика на датчике выключения 2
};

}//namespace hydro

namespace servo
{

class Parameters : public test::Parameters
{
public:
    static Parameters& Instance();

    void Reset();
    QString ToString();

    bool ReelControl ( QString const& val );
    RELL_CONTROL const& ReelControl () const;

    bool PressureNominal ( QString const& val );
    qint32 const& PressureNominal () const;

    bool PressureTesting ( QString const& val );
    qint32 const& PressureTesting () const;

    bool MaxExpenditureA ( QString const& val );
    qint32 const& MaxExpenditureA () const;

    bool MaxExpenditureB ( QString const& val );
    qint32 const& MaxExpenditureB () const;

    bool FrequencyInc ( QString const& val );
    const double_t &FrequencyInc() const;

private:
    Parameters();
    Parameters( Parameters const& )  = delete;
    void operator = ( Parameters& ) = delete;

    RELL_CONTROL mReelControl;
    qint32 mPressureNominal;
    qint32 mPressureTesting;
    qint32 mMaxExpenditureA;
    qint32 mMaxExpenditureB;
    double_t mFrequencyInc;
};

}//namespace servo

}//namespace test
