#pragma once
#include "../test_base.h"

namespace test
{
namespace pump
{
class PumpTest8 :public test::pump::Test
{
public:
   PumpTest8();
   bool Run();

   QJsonObject Serialise() const;
   bool Deserialize( QJsonObject const& obj );

   bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

   bool Success() const;
virtual QString RepRes() const;
virtual QString RepName() const;
private:
   double mExpMin1 = 0.0;
   double mExpNom1 = 0.0;
   double mExpMax1 = 0.0;

   double mExpMin2 = 0.0;
   double mExpNom2 = 0.0;
   double mExpMax2 = 0.0;

   double mFrequencyMin = 0.0;
   double mFrequencyMax = 0.0;
   double mFrequencyNom = 0.0;
};

}//namespace pump
}//namespace test
