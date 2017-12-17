#pragma once
#include "../test_base.h"
#include <condition_variable>

namespace test
{
namespace pump
{
class PumpTest3 :public test::pump::Test
{
public:
   PumpTest3();
   bool Run();

   QJsonObject Serialise() const;
   bool Deserialize( QJsonObject const& obj );

   bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

   bool Success() const;
   virtual QString RepRes() const;
   virtual QString RepName() const;

private:
   bool mResult;
   std::condition_variable mCondVar;
   void Question();
};

}//namespace pump
}//namespace test
