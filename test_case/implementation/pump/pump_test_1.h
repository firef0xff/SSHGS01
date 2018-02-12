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
   bool mResult = false;
   bool mIsSet = false;
   bool mInvalidExpenditure1 = false;
   bool mInvalidExpenditure2 = false;

   double mMaxExp1 = 0.0;
   double mMinExp1 = 0.0;
   double mMaxExp2 = 0.0;
   double mMinExp2 = 0.0;


   std::condition_variable mCondVar;
   void Question();

   cpu::data::M2& mContol;
};

}//namespace pump
}//namespace test
