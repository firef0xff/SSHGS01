#pragma once
#include "../test_base.h"

namespace test
{
namespace pump
{
class PumpTest2 :public test::pump::Test
{
public:
   PumpTest2();
   bool Run();

   QJsonObject Serialise() const;
   bool Deserialize( QJsonObject const& obj );

   bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

   bool Success() const;
   virtual QString RepRes();
   virtual QString RepName();
private:
   bool mResult;
};

}//namespace pump
}//namespace test
