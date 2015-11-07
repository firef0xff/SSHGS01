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
    QString ToString() const;

    test::TestCase& TestCollection() const;

    QJsonObject Serialise() const;
    bool Deserialize(const QJsonObject &obj );

    void WriteToController() const;

    bool Draw(QPainter &painter, QRect &free_rect ) const;
    bool DrawResults(QPainter &painter, QRect &free_rect ) const;

    bool GsType ( QString const& val );
    QString const& GsType () const;

    bool Voltage ( QString const& val );
    const double &Voltage() const;

    bool VoltageRange ( QString const& val );
    const double &VoltageRange() const;

    bool Lost ( QString const& val );
    const double &Lost() const;

    bool VoltageType ( QString const& val );
    const VOLTAGE_TYPE &VoltageType() const;

    bool MaxWorkPressure ( QString const& val );
    const double &MaxWorkPressure() const;

    bool MinTestPressure ( QString const& val );
    const double &MinTestPressure() const;

    bool HermPressure ( QString const& val );
    const double &HermPressure() const;

    bool TestControlPressure ( QString const& val );
    const double &TestControlPressure() const;

    bool HermSignal ( QString const& val );
    CONTROL_SIGNAL const& HermSignal () const;

    bool PABTSignal ( QString const& val );
    CONTROL_SIGNAL const& PABTSignal () const;

    bool PBATSignal ( QString const& val );
    CONTROL_SIGNAL const& PBATSignal () const;

    bool ActuationOnTime ( QString const& val );
    const double &ActuationOnTime() const;

    bool ActuationOffTime ( QString const& val );
    double const& ActuationOffTime () const;

    bool OnDD1A ( Qt::CheckState const& val );
    bool const& OnDD1A () const;
    bool OnDD2A ( Qt::CheckState const& val );
    bool const& OnDD2A () const;
    bool OnDD3A ( Qt::CheckState const& val );
    bool const& OnDD3A () const;

    bool OffDD1A ( Qt::CheckState const& val );
    bool const& OffDD1A () const;
    bool OffDD2A ( Qt::CheckState const& val );
    bool const& OffDD2A () const;
    bool OffDD3A ( Qt::CheckState const& val );
    bool const& OffDD3A () const;

    bool OnDD1B ( Qt::CheckState const& val );
    bool const& OnDD1B () const;
    bool OnDD2B ( Qt::CheckState const& val );
    bool const& OnDD2B () const;
    bool OnDD3B ( Qt::CheckState const& val );
    bool const& OnDD3B () const;

    bool OffDD1B ( Qt::CheckState const& val );
    bool const& OffDD1B () const;
    bool OffDD2B ( Qt::CheckState const& val );
    bool const& OffDD2B () const;
    bool OffDD3B ( Qt::CheckState const& val );
    bool const& OffDD3B () const;


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
    double mVoltage;            //+Напряжение питания
    double mVoltageRange;       //+Допустимое отклонение напрядения питания
    double mLost;               //+Допустимое значение утечки
    VOLTAGE_TYPE mVoltageType;  //+тип напряжения
    double mMaxWorkPressure;    //+максимальное рабочее давление

    double mMinTestPressure;    //+давление для испытания функционирования минимальным давлением
    double mHermPressure;       //+давление для испытания герметичности
    double mTestControlPressure;//+давление для испытания времени переключения

    CONTROL_SIGNAL mHermSignal; //+сигнал проверки внутренней герметичности
    CONTROL_SIGNAL mPABTSignal; //+сигнал PABT
    CONTROL_SIGNAL mPBATSignal; //+сигнал PBAT

    double mActuationOnTime;    //+время срабатывания включения распределителя
    double mActuationOffTime;   //+время срабатывания выключения распределителя

    bool mOnDD1A;   //+Используемый датчик для определения динамики включения
    bool mOnDD2A;   //+Используемый датчик для определения динамики включения
    bool mOnDD3A;   //+Используемый датчик для определения динамики включения

    bool mOffDD1A;  //+Используемый датчик для определения динамики выключения
    bool mOffDD2A;  //+Используемый датчик для определения динамики выключения
    bool mOffDD3A;  //+Используемый датчик для определения динамики выключения

    bool mOnDD1B;   //+Используемый датчик для определения динамики включения
    bool mOnDD2B;   //+Используемый датчик для определения динамики включения
    bool mOnDD3B;   //+Используемый датчик для определения динамики включения

    bool mOffDD1B;  //+Используемый датчик для определения динамики выключения
    bool mOffDD2B;  //+Используемый датчик для определения динамики выключения
    bool mOffDD3B;  //+Используемый датчик для определения динамики выключения

    DYNAMIC mOnDynamic_1;    //+ожидаемая динамика на датчике включения 1
    DYNAMIC mOffDynamic_1;   //+ожидаемая динамика на датчике выключения 1
    DYNAMIC mOnDynamic_2;    //+ожидаемая динамика на датчике включения 2
    DYNAMIC mOffDynamic_2;   //+ожидаемая динамика на датчике выключения 2
};

}//namespace hydro
}//namespace test
