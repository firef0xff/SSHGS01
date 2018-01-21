#pragma once
#include "../test_base.h"

namespace test
{
namespace pump
{
class PumpTest4 :public test::pump::Test
{
public:
   PumpTest4();
   bool Run();

   QJsonObject Serialise() const;
   bool Deserialize( QJsonObject const& obj );

   bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

   bool Success() const;
   virtual QString RepRes() const;
   virtual QString RepName() const;
private:
   double mData;
};

}//namespace pump
}//namespace test
