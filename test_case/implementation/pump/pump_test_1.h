#pragma once
#include "../test_base.h"
#include <condition_variable>

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

   void UpdateData();

   QString RepRes() const;
   QString RepName() const;

private:
   bool mResult;
   bool mIsSet;
   std::condition_variable mCondVar;
   void Question();

   cpu::data::M2& mContol;
};

}//namespace pump
}//namespace test
