#pragma once
#include "../test_params.h"

namespace test
{

namespace pump
{

enum SPIN
{
   spUnknown,
   spLeft,
   spRight
};

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

   qint32 mFuncTestTime = 0; //Продолжительность испытаний функционирования
   qint32 mStrongTestTime = 0; //Продолжительность испытаний на прочность
   qint32 mHermTestTime = 0; //Продолжительность испытаний наружной герметичности

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
   qint32 mSpinSpeedMin = 0; //Минимальная
   qint32 mSpinSpeedNom = 0; //Номинальная
   qint32 mSpinSpeedMax = 0; //Максимальная

   double mVolumeKPD = 0.0; //Объемный кпд насоса
   double mExpenditure = 0.0; //Дренаж насоса при номинальном давлении и номинальном расходе л/мин

   //Сервисные настройки для испытания насосов:
   double mX;//Время набора требуемого давления, сек (X)
   double mY;//Время набора требуемой частоты вращения, сек (Y)
   double mZ;//Время стабилизации потока, сек (Z)
   double mA;//Коэффициент функционирования, % (А)
   double mA1;//Коэффициент функционирования для дренажа, % (А1)
   double mE;//Точность для испытаний функциональные зависимости, сек (Е)
   double mB;//Продолжительность испытаний функциональные зависимости, сек (В)
};

}
}
