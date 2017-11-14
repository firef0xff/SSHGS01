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

   QString mSerianNo = "";   //Идентификационный номер
   SPIN mSpin = spUnknown;   //Направление вращения
   double mWorkVolume = 0.0; //Рабочий объем

   //давление, бар
   double mPressureMin = 0.0; //Минимальное
   double mPressureNom = 0.0; //номинальное (рабочее)
   double mPressureMax = 0.0; //максимальное

   //Частота вращения об/мин
   qint32 mSpinSpeedMin = 0; //Минимальная
   qint32 mSpinSpeedNom = 0; //Номинальная
   qint32 mSpinSpeedMax = 0; //Максимальная

   double mVolumeKPD = 0.0; //Объемный кпд насоса
   double mExpenditure = 0.0; //Дренаж насоса при номинальном режиме л/мин
};

}
}
