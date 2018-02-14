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


   typedef double ArrData;
   typedef std::vector< double > DataSet;
   typedef std::pair< double, DataSet > SourceItem;
   typedef std::vector< DataSet > Source;

   PumpTest10();
   ~PumpTest10();
   bool Run();

   QJsonObject Serialise() const;
   bool Deserialize( QJsonObject const& obj );

   bool Draw(QPainter &painter, QRect &free_rect, const QString & ) const;

   bool Success() const;
   bool RepSkeep();

   void ResetDrawLine();

   void UpdateData();

private:
   friend class GrapfData;

   bool mResult;

   Source mPressure_S1;         //Давление в полости 1
   Source mPressure_S2;         //Давление в полости 2
   Source mExpenditure_S1;   //подача
   Source mExpenditure_S2;   //подача
   Source mExpCoeff_S1;      //коэффициент подачи
   Source mExpCoeff_S2;      //коэффициент подачи
   Source mPower;         //мощность
   Source mKPD;           //КПД
   DataSet mSpeenSpeedMap;

   int key;
   bool ready_for_ready;
   mutable std::unique_ptr<GrapfData> mGrapfs;
};

}//namespace pump
}//namespace test
