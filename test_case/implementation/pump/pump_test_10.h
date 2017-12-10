#pragma once
#include "../test_base.h"
#include <memory>

namespace test
{
namespace pump
{
class PumpTest10 :public test::pump::Test
{
public:
   class GrapfData;
   struct ArrData
   {
       double mPower = 0.0;         //мощность
       double mExpenditure = 0.0;   //подача
       double mExpCoeff = 0.0;      //коэффициент подачи
       double mKPD = 0.0;           //КПД
       double mPressure = 0.0;      //Давление (ось х)
   };
   typedef std::vector< ArrData > DataSet;
   typedef std::pair< const double , DataSet > SourceItem;
   typedef std::map<const double , DataSet> Source; //частота вращения, зависимость

   PumpTest10();
   ~PumpTest10();
   bool Run();

   QJsonObject Serialise() const;
   bool Deserialize( QJsonObject const& obj );

   bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

   bool Success() const;
   bool RepSkeep();

   void ResetDrawLine();

private:
   friend class GrapfData;

   bool mResult;
   Source mSourse;
   mutable std::unique_ptr<GrapfData> mGrapfs;
};

}//namespace pump
}//namespace test
