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
   double mExpMin = 0.0;
   double mExpNom = 0.0;
   double mExpMax = 0.0;

   double mFrequencyMin = 0.0;
   double mFrequencyMax = 0.0;
   double mFrequencyNom = 0.0;
};

}//namespace pump
}//namespace test
