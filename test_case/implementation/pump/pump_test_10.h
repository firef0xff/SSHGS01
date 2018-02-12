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


   typedef std::pair< double, double > ArrData;
   typedef std::vector< ArrData > DataSet;
   typedef std::pair< double, DataSet > SourceItem;
   typedef std::map< double, DataSet > Source;

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

   Source mPower;         //мощность
   Source mExpenditure;   //подача
   Source mExpCoeff;      //коэффициент подачи
   Source mKPD;           //КПД
   mutable std::unique_ptr<GrapfData> mGrapfs;
};

}//namespace pump
}//namespace test
