#pragma once
#include "../test_params.h"

namespace test
{
namespace hydro
{

class Parameters : public test::CommonParameters
{
public:
    static Parameters& Instance();

    void Reset();
    QString ToString();

    test::TestCase& TestCollection() const;

    QJsonObject Serialise() const;
    bool Deserialize(const QJsonObject &obj );

    bool GsType ( QString const& val );
    QString const& GsType () const;

    bool Voltage ( QString const& val );
    qint32 const& Voltage () const;

    bool VoltageRange ( QString const& val );
    qint32 const& VoltageRange () const;

    bool Lost ( QString const& val );
    const double &Lost() const;

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
    double mLost;             //Допустимое значение утечки
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
}//namespace test
