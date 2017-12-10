#pragma once
#include "../test_base.h"

namespace test
{
namespace pump
{
class PumpTest7 :public test::pump::Test
{
public:
   PumpTest7();
   bool Run();

   QJsonObject Serialise() const;
   bool Deserialize( QJsonObject const& obj );

   bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

   bool Success() const;
   QString RepRes();
   QString RepName();
private:
   bool mResult;
   double mData;
};

}//namespace pump
}//namespace test
