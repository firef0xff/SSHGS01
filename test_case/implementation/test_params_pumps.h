#pragma once
#include "../test_params.h"

namespace test
{

enum SPIN
{
   spUnknown,
   spLeft,
   spRight
};

bool ParseValue ( SPIN& sig, QString const& val );
QString ToString( SPIN const& v );

namespace pump
{

class Parameters : public test::Parameters
{
public:
   static Parameters& Instance();

   void Reset();
   QString ToString() const;

   test::TestCase& TestCollection() const;

   QJsonObject Serialise() const;
   bool Deserialize(const QJsonObject &obj );

   void WriteToController() const;

   bool Draw(QPainter &painter, QRect &free_rect, QString const& compare_width ) const;
   bool DrawResults(QPainter &painter, QRect &free_rect ) const;

   QString ModelId() const;
   virtual bool HasResults()
   {
       return true;
   }

   //Параметры насоса
   bool SerianNo( QString const& val );
   const QString &SerianNo();
   bool Model( QString const& val );
   const QString &Model();
  //Ответственный User()
   bool SectionsCount( qint32 val );
   qint32 SectionsCount();
   bool Spin( QString const& val );
   SPIN Spin();

   bool ElektrikControl( bool val );
   bool ElektrikControl();
   bool Voltage( QString const& val );
   double Voltage();
   bool VoltageType( QString const& val );
   VOLTAGE_TYPE VoltageType();

   bool HydroControl( bool val );
   bool HydroControl();
   bool MaxControlPressure( QString const& val );
   double MaxControlPressure();

   bool     WorkVolume1( QString const& val );
   double   WorkVolume1();
   bool     PressureMin1( QString const& val );
   double   PressureMin1();
   bool     PressureNom1( QString const& val );
   double   PressureNom1();
   bool     PressureMax1( QString const& val );
   double   PressureMax1();

   bool     WorkVolume2( QString const& val );
   double   WorkVolume2();
   bool     PressureMin2( QString const& val );
   double   PressureMin2();
   bool     PressureNom2( QString const& val );
   double   PressureNom2();
   bool     PressureMax2( QString const& val );
   double   PressureMax2();

   bool     FrequencyMin( QString const& val );
   qint32   FrequencyMin();
   bool     FrequencyNom( QString const& val );
   qint32   FrequencyNom();
   bool     FrequencyMax( QString const& val );
   qint32   FrequencyMax();

   bool     VolumeKPD( QString const& val );
   double   VolumeKPD();
   bool     Expenditure( QString const& val );
   double   Expenditure();

   //параметры испытания
   bool FuncTestTime( QString const& val );
   qint32 FuncTestTime();
   bool StrongTestTime( QString const& val );
   qint32 StrongTestTime();
   bool HermTestTime( QString const& val );
   qint32 HermTestTime();

   bool X( QString const& val );
   qint32 X();
   bool Y( QString const& val );
   qint32 Y();
   bool Z( QString const& val );
   qint32 Z();
   bool A( QString const& val );
   double A();
   bool A1( QString const& val );
   double A1();
   bool E( QString const& val );
   double E();
   bool B( QString const& val );
   qint32 B();

private:
    Parameters();
    Parameters( Parameters const& ) = delete;
    void operator = ( Parameters& ) = delete;

    //Перед началом испытания необходимо задать следующие параметры:
   QString mSerianNo = "";   //Серийный номер //Идентификационный номер
   QString mModel = ""; //Модель гидронасоса
   //Ответственный User()
   qint32 mSectionsCount = 0; //Количество секций в насосе
   SPIN mSpin = spUnknown;   //Направление вращения

   bool mElektrikControl = false; //Наличие электронного управления
   double mVoltage = 0; //Напряжение питания, В
   VOLTAGE_TYPE mVoltageType = VT_UNKNOWN; //Тип тока

   bool mHydroControl = false; //Наличие гидравлического управления
   double mMaxControlPressure = 0; //Максимальное давление в канале управления, бар

   double mWorkVolume1 = 0.0; //Рабочий объем насоса №1

   //Давление в напорной полости насоса №1:
   double mPressureMin1 = 0.0; //Минимальное
   double mPressureNom1 = 0.0; //номинальное (рабочее)
   double mPressureMax1 = 0.0; //максимальное

   double mWorkVolume2 = 0.0; //Рабочий объем насоса №2

   //Давление в напорной полости насоса №2
   double mPressureMin2 = 0.0; //Минимальное
   double mPressureNom2 = 0.0; //номинальное (рабочее)
   double mPressureMax2 = 0.0; //максимальное

   //Частота вращения насоса(ов):
   qint32 mFrequencyMin = 0; //Минимальная
   qint32 mFrequencyNom = 0; //Номинальная
   qint32 mFrequencyMax = 0; //Максимальная

   double mVolumeKPD = 0.0; //Объемный кпд насоса
   double mExpenditure = 0.0; //Дренаж насоса при номинальном давлении и номинальном расходе л/мин

   //Сервисные настройки для испытания насосов:
   qint32 mFuncTestTime = 0; //Продолжительность испытаний функционирования
   qint32 mStrongTestTime = 0; //Продолжительность испытаний на прочность
   qint32 mHermTestTime = 0; //Продолжительность испытаний наружной герметичности

   qint32 mX = 0;//Время набора требуемого давления, сек (X)
   qint32 mY = 0;//Время набора требуемой частоты вращения, сек (Y)
   qint32 mZ = 0;//Время стабилизации потока, сек (Z)
   double mA = 0.0;//Коэффициент функционирования, % (А)
   double mA1 = 0.0;//Коэффициент функционирования для дренажа, % (А1)
   double mE = 0.0;//Точность для испытаний функциональные зависимости, сек (Е)
   qint32 mB = 0;//Продолжительность испытаний функциональные зависимости, сек (В)
};

}
}
