#pragma once
#include "../test_base.h"

namespace test
{
namespace pump
{
class PumpTest1 :public test::pump::Test
{
public:
   PumpTest1();
   bool Run();

   QJsonObject Serialise() const;
   bool Deserialize( QJsonObject const& obj );

   bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

   bool Success() const;

private:
   bool mResult;
};

}//namespace pump
}//namespace test
