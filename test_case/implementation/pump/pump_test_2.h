#pragma once
#include "../test_base.h"
#include <condition_variable>

namespace test
{
namespace pump
{
class PumpTest2 :public test::pump::Test
{
public:
   PumpTest2();
   bool Run();

   void UpdateData();

   QJsonObject Serialise() const;
   bool Deserialize( QJsonObject const& obj );

   bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

   bool Success() const;
   virtual QString RepRes() const;
   virtual QString RepName() const;

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
