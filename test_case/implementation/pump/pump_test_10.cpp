#include "pump_test_10.h"
#include <QJsonObject>
#include "../test_params_pumps.h"
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
#include <thread>
namespace test
{
namespace pump
{

namespace
{
QJsonArray ToJson( PumpTest10::DataSet const& in_src )
{
    QJsonArray source;
    for ( auto it = in_src.begin(), end = in_src.end(); it != end; ++it )
    {
       PumpTest10::ArrData const& lnk = *it;
       source.insert( source.end(), lnk );
    }

    return source;
}
PumpTest10::DataSet DataSetFromJson( QJsonArray arr )
{
   PumpTest10::DataSet ds;
   foreach ( QJsonValue const& v_item, arr )
   {
      PumpTest10::ArrData it = v_item.toDouble();
      ds.push_back( it );
   }
   return ds;
}


QJsonArray ToJson( PumpTest10::Source const& in_src )
{
    QJsonArray source;
    for ( auto it = in_src.begin(), end = in_src.end(); it != end; ++it )
    {
        PumpTest10::DataSet const& data = *it;
        QJsonArray data_set = ToJson( data );
        source.insert( source.end(), data_set );
    }

    return source;
}
PumpTest10::Source FromJson( QJsonArray arr )
{
    PumpTest10::Source res;
    foreach (QJsonValue const& v, arr)
    {
        QJsonArray data_set = v.toArray();
        PumpTest10::DataSet ds = DataSetFromJson( data_set );
        res.push_back( ds );
    }
    return std::move( res );
}




ff0x::NoAxisGraphBuilder::LinePoints Process ( PumpTest10::DataSet const& x_src,
                                               PumpTest10::DataSet const& y_src,
                                               QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( size_t i = 0; i < x_src.size() && i < y_src.size(); ++i )
    {
        double const x = fabs( x_src[i] );
        double const y = fabs( y_src[i] );

        if ( !i )
        {
            x_range.setX( x );
            x_range.setY( x );
            y_range.setX( y );
            y_range.setY( y );
        }
        else
        {
            if ( x > x_range.x() )
                x_range.setX( x );
            if ( x < x_range.y() )
                x_range.setY( x );

            if ( y > y_range.x() )
                y_range.setX( y );
            if ( y < y_range.y() )
                y_range.setY( y );
        }

        result.push_back( QPointF( x, y ) );
    }
    return std::move( result );
}
ff0x::NoAxisGraphBuilder::LinePoints Process2 ( PumpTest10::DataSet const& x1_src,
                                                PumpTest10::DataSet const& x2_src,
                                                PumpTest10::DataSet const& y_src,
                                                QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( size_t i = 0; i < x1_src.size() && i < x2_src.size() && i < y_src.size(); ++i )
    {
        double const x = max( fabs( x1_src[i] ), fabs( x2_src[i] ) );
        double const y = fabs( y_src[i] );

        if ( !i )
        {
            x_range.setX( x );
            x_range.setY( x );
            y_range.setX( y );
            y_range.setY( y );
        }
        else
        {
            if ( x > x_range.x() )
                x_range.setX( x );
            if ( x < x_range.y() )
                x_range.setY( x );

            if ( y > y_range.x() )
                y_range.setX( y );
            if ( y < y_range.y() )
                y_range.setY( y );
        }

        result.push_back( QPointF( x, y ) );
    }
    return std::move( result );
}

}

class PumpTest10::GrapfData
{
public:
   GrapfData( PumpTest10 const* test, QString compare_width )
   {
      mColorMap = { Qt::blue, Qt::darkBlue, Qt::gray, Qt::darkGray };
      mData.resize(6);
      //поиск данных теста
      PumpTest10::Source pressure_S1;
      PumpTest10::Source pressure_S2;
      PumpTest10::Source expenditure_S1;
      PumpTest10::Source expenditure_S2;
      PumpTest10::Source exp_coeff_S1;
      PumpTest10::Source exp_coeff_S2;
      PumpTest10::Source power;
      PumpTest10::Source kpd;

      PumpTest10::DataSet speen_speed_tmpl;

      foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
      {
         auto obj = val.toObject();
         if ( obj.value("id").toInt() == test->mId )
         {
            auto data = obj.value("data").toObject();

            pressure_S1 = FromJson( data.value("mPressure_S1").toArray() );
            pressure_S2 = FromJson( data.value("mPressure_S2").toArray() );
            expenditure_S1 = FromJson( data.value("mExpenditure_S1").toArray() );
            expenditure_S2 = FromJson( data.value("mExpenditure_S2").toArray() );
            exp_coeff_S1 = FromJson( data.value("mExpCoeff_S1").toArray() );
            exp_coeff_S2 = FromJson( data.value("mExpCoeff_S2").toArray() );
            kpd = FromJson( data.value("mKPD").toArray() );
            power = FromJson( data.value("mPower").toArray() );
            speen_speed_tmpl =  DataSetFromJson( data.value("mSpeenSpeedMap").toArray() );
         }
      }

      auto ProcessTmplData2 = [this, &pressure_S1, &pressure_S2, &speen_speed_tmpl]( PumpTest10::Source const& src, int pos )
      {
         for( size_t k = 0; k < src.size(); ++k )
         {
            PumpTest10::DataSet const& x1_src = pressure_S1[k];
            PumpTest10::DataSet const& x2_src = pressure_S2[k];
            PumpTest10::DataSet const& y_src = src[k];
            Data &d = mData[pos][k];
            d.templdate = Process2( x1_src, x2_src, y_src , d.x_range, d.y_range );
            d.templSpeenSpeed = speen_speed_tmpl[k];
         }
      };
      auto ProcessTmplData = [this, &speen_speed_tmpl]( PumpTest10::Source const& xsrc, PumpTest10::Source const& src, int pos )
      {
         for( size_t k = 0; k < src.size(); ++k )
         {
            PumpTest10::DataSet const& x_src = xsrc[k];

            PumpTest10::DataSet const& y_src = src[k];
            Data &d = mData[pos][k];
            d.templdate = Process( x_src, y_src , d.x_range, d.y_range );
            d.templSpeenSpeed = speen_speed_tmpl[k];
         }
      };

      auto const& ps1 = test->mPressure_S1;
      auto const& ps2 = test->mPressure_S2;
      auto ProcessData2 = [this, &ps1, &ps2]( PumpTest10::Source const& src, int pos )
      {
         for( size_t k = 0; k < src.size(); ++k )
         {
            QPointF x_range;
            QPointF y_range;

            PumpTest10::DataSet const& x1_src = ps1[k];
            PumpTest10::DataSet const& x2_src = ps2[k];

            PumpTest10::DataSet const& y_src = src[k];
            Data &d = mData[pos][k];
            d.data = Process2( x1_src, x2_src, y_src , x_range, y_range );
            d.x_range = ff0x::MergeRanges( x_range, d.x_range, !d.templdate.empty() );
            d.y_range = ff0x::MergeRanges( y_range, d.y_range, !d.templdate.empty() );
         }
      };
      auto ProcessData = [this]( PumpTest10::Source const& xsrc, PumpTest10::Source const& src, int pos )
      {
         for( size_t k = 0; k < src.size(); ++k )
         {
            QPointF x_range;
            QPointF y_range;

            PumpTest10::DataSet const& x_src = xsrc[k];
            PumpTest10::DataSet const& y_src = src[k];
            Data &d = mData[pos][k];
            d.data = Process( x_src, y_src , x_range, y_range );
            d.x_range = ff0x::MergeRanges( x_range, d.x_range, !d.templdate.empty()  );
            d.y_range = ff0x::MergeRanges( y_range, d.y_range, !d.templdate.empty()  );
         }
      };

      ProcessTmplData2( power, 0 );
      ProcessData2( test->mPower, 0 );

      ProcessTmplData2( kpd, 1 );
      ProcessData2( test->mKPD, 1 );

      ProcessTmplData( pressure_S1, expenditure_S1, 2 );
      ProcessData( test->mPressure_S1, test->mExpenditure_S1, 2 );

      ProcessTmplData( pressure_S2, expenditure_S2, 3 );
      ProcessData( test->mPressure_S2, test->mExpenditure_S2, 3 );

      ProcessTmplData( pressure_S1, exp_coeff_S1, 4 );
      ProcessData( test->mPressure_S1, test->mExpCoeff_S1, 4 );

      ProcessTmplData( pressure_S2, exp_coeff_S2, 5 );
      ProcessData( test->mPressure_S2, test->mExpCoeff_S2, 5 );
   }

    struct Data
    {
       ff0x::NoAxisGraphBuilder::LinePoints data;
       ff0x::NoAxisGraphBuilder::LinePoints templdate;

       double templSpeenSpeed = 0.0;

       QPointF x_range;
       QPointF y_range;
    };
    typedef std::map< int, Data > Grapfs; //массивы графиков сортированняе по частоте вращения насоса
    typedef std::vector< Grapfs > DataSet; //вектор графиков по типам

    DataSet mData;
    typedef std::vector<Qt::GlobalColor> ColorMap;
    ColorMap mColorMap;
};

PumpTest10::PumpTest10():
    test::pump::Test( "Функциональные зависимости", 49 ),
    mResult(false),
    key(0),
    ready_for_ready(true)
{
   mSpeenSpeedMap = { 0, 0 };
}

PumpTest10::~PumpTest10()
{}

bool PumpTest10::Run()
{
    key = 0;
    ready_for_ready = true;
    mPressure_S1.clear();
    mPressure_S2.clear();
    mExpenditure_S1.clear();
    mExpenditure_S2.clear();
    mExpCoeff_S1.clear();
    mExpCoeff_S2.clear();
    mPower.clear();
    mKPD.clear();
    mSpeenSpeedMap = { 0, 0 };

    Start();
    Wait( mBits.OP49_Work, mBits.OP49_End );
    mResult = true;
    if ( IsStopped() )
        mResult = false;
    return Success();
}

void PumpTest10::UpdateData()
{
   Test::UpdateData();

   mSpeenSpeedMap = { mBits.OP49_SpeenSpeed1, mBits.OP49_SpeenSpeed2 };

   if ( !mBits.OP49_Ready )
   {
      ready_for_ready = true;
      return;
   }

   if ( !ready_for_ready )
       return;

   ready_for_ready = false;

   std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

   auto& mem = cpu::CpuMemory::Instance().DB90;
   mem.Read();

   int count = mBits.OP49_Count;
   int k = key;
   ++key;

   mPressure_S1.resize(k+1);
   mPressure_S2.resize(k+1);
   mExpenditure_S1.resize(k+1);
   mExpenditure_S2.resize(k+1);
   mExpCoeff_S1.resize(k+1);
   mExpCoeff_S2.resize(k+1);
   mPower.resize(k+1);
   mKPD.resize(k+1);

   mPressure_S1[k].clear();
   mPressure_S2[k].clear();
   mExpenditure_S1[k].clear();
   mExpenditure_S2[k].clear();
   mExpCoeff_S1[k].clear();
   mExpCoeff_S2[k].clear();
   mPower[k].clear();
   mKPD[k].clear();

   for ( int i = 0; i < count; ++i )
   {
      mPressure_S1[k].push_back( mem.Pressure_S1[i] );
      mPressure_S2[k].push_back( mem.Pressure_S2[i] );
      mExpenditure_S1[k].push_back( mem.Consumption_S1[i] );
      mExpenditure_S2[k].push_back( mem.Consumption_S2[i] );
      mExpCoeff_S1[k].push_back( mem.K_Consumption_S1[i] );
      mExpCoeff_S2[k].push_back( mem.K_Consumption_S2[i] );
      mPower[k].push_back( mem.Power[i] );
      mKPD[k].push_back( mem.KPD[i] );
   }

   auto& cnt = cpu::CpuMemory::Instance().M2;
   cnt.Pump_Continue = true;
   cnt.Pump_Fail = false;
   cnt.Write();
}

QJsonObject PumpTest10::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mResult",            mResult );
    obj.insert("mPressure_S1",    ToJson(mPressure_S1) );
    obj.insert("mPressure_S2",    ToJson(mPressure_S2) );
    obj.insert("mExpenditure_S1",    ToJson(mExpenditure_S1) );
    obj.insert("mExpenditure_S2",    ToJson(mExpenditure_S2) );
    obj.insert("mExpCoeff_S1",    ToJson(mExpCoeff_S1) );
    obj.insert("mExpCoeff_S2",    ToJson(mExpCoeff_S2) );
    obj.insert("mKPD",    ToJson(mKPD) );
    obj.insert("mPower",    ToJson(mPower) );

    obj.insert("mSpeenSpeedMap",    ToJson(mSpeenSpeedMap) );

    return obj;
}
bool PumpTest10::Deserialize( QJsonObject const& obj )
{
    mResult = obj.value("mResult").toBool();
    mPressure_S1 = FromJson( obj.value("mPressure_S1").toArray() );
    mPressure_S2 = FromJson( obj.value("mPressure_S2").toArray() );
    mExpenditure_S1 = FromJson( obj.value("mExpenditure_S1").toArray() );
    mExpenditure_S2 = FromJson( obj.value("mExpenditure_S2").toArray() );
    mExpCoeff_S1 = FromJson( obj.value("mExpCoeff_S1").toArray() );
    mExpCoeff_S2 = FromJson( obj.value("mExpCoeff_S2").toArray() );
    mKPD = FromJson( obj.value("mKPD").toArray() );
    mPower = FromJson( obj.value("mPower").toArray() );

    mSpeenSpeedMap = DataSetFromJson( obj.value("mSpeenSpeedMap").toArray() );

    Test::Deserialize( obj );
    return true;
}

bool PumpTest10::Success() const
{
    return mResult;
}
bool PumpTest10::RepSkeep()
{
   return true;
}
void PumpTest10::ResetDrawLine()
{
    Test::ResetDrawLine();
    mGrapfs.reset();
}

bool PumpTest10::Draw(QPainter& painter, QRect &free_rect , const QString &compare_width) const
{
   test::pump::Parameters *params = static_cast< test::pump::Parameters * >( CURRENT_PARAMS );
   if ( !params )
      return true;

   QFont header_font = painter.font();
   header_font.setFamily("Arial");
   header_font.setPointSize( 14 );
   QFont result_font = header_font;
   result_font.setUnderline(true);
   QFont text_font = header_font;
   text_font.setPointSize( 12 );

   DrawHelper drw( painter, free_rect );

   QFontMetrics m(text_font);
   int width = m.width("123456789012345678901234567890123456789012345");
   char symbol = '.';
   auto FillToSize = [ width, &m, symbol ]( QString text )
   {
     while( m.width( text + symbol ) < width )
         text += symbol;
     return text + " ";
   };


   uint32_t num = 0;
   bool res = DrawLine( num, free_rect, header_font,
   [ this, &drw, &header_font ]( QRect const& rect )
   {
      drw.DrawRowCenter( rect, header_font, Qt::black, "10."+mName );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
      drw.DrawRowLeft( r, text_font, Qt::black, "Цель данного испытания - построение графиков зависимости:" );
   }, 1.5 );   
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "1) Мощности насоса от давления при различной частоте вращения насоса;" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "2) КПД от давления при различной частоте вращения насоса;" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "3) Подачи насоса от давления при различной частоте вращения насоса;" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "4) Коэффициент подачи от давления при различной частоте вращения насоса;" );
   }, 1.5 );



   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});

   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &result_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, result_font, Qt::black, "Параметры во время испытаний:" );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font, &params ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Длительность испытания, сек"), Qt::red, test::ToString(TestingTime) );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &result_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, result_font, Qt::black, "РЕЗУЛЬТАТ:" );
   }, 1.5 );

   //Графики
   QFontMetrics metrix( text_font );
   if (!mGrapfs)
       mGrapfs.reset( new GrapfData( this, compare_width ) );

   typedef PumpTest10::GrapfData::Grapfs Grafs;
   typedef PumpTest10::GrapfData::ColorMap ColorMap;

   auto DrawGrafs = [this, &num, &free_rect, &text_font, &painter, &drw, &metrix]
         ( int grapfs_index, QString const& title, QString const& x_msg, QString const& y_msg )
   {
      return DrawLine( num, free_rect, text_font,
           [ this, &painter, &text_font, &drw, &metrix, &grapfs_index, &title, &x_msg, &y_msg ]( QRect const& rect )
           {
               painter.save();

               QFont f = text_font;
               f.setPointSize( 12 );
               int w = (rect.width())*0.98;
               int h = (rect.height() - metrix.height())*0.98;

               ff0x::NoAxisGraphBuilder builder ( w, h, f );
               builder.DrawBorderCeil(false);
               ff0x::NoAxisGraphBuilder::GraphDataLine lines;
               QPointF x_range;
               QPointF y_range;
               Grafs const& grapfs = mGrapfs->mData[grapfs_index];
               ColorMap const& colors = mGrapfs->mColorMap;
               for ( auto it = grapfs.begin(), end = grapfs.end(); it != end; ++it )
               {
                  PumpTest10::GrapfData::Data const& data = it->second;
                  if ( it == grapfs.begin() )
                  {
                     x_range = data.x_range;
                     y_range = data.y_range;
                  }
                  else
                  {
                     x_range = ff0x::MergeRanges( x_range, data.x_range );
                     y_range = ff0x::MergeRanges( y_range, data.y_range );
                  }

                  int key = it->first;
                  lines.push_back( ff0x::NoAxisGraphBuilder::Line(data.data, ff0x::NoAxisGraphBuilder::LabelInfo( test::ToString(mSpeenSpeedMap[key]) + " об/мин", colors[key] ) ) );
                  if ( !data.templdate.isEmpty() )
                     lines.push_back( ff0x::NoAxisGraphBuilder::Line(data.templdate, ff0x::NoAxisGraphBuilder::LabelInfo( "Эталон " + test::ToString(data.templSpeenSpeed) + " об/мин", colors[key+2] ) ) );
               }

               QRect p1(rect.left(), rect.top(), w, h );
               QRect p1t(p1.left(), p1.bottom(), p1.width(), metrix.height());
               drw.DrawRowCenter( p1t, text_font, Qt::black, title );


               QPointF x_grapf_range;
               QPointF y_grapf_range;
               double x_step = 0;
               double y_step = 0;

               ff0x::DataLength( x_range,x_grapf_range, x_step );
               ff0x::DataLength( y_range,y_grapf_range, y_step );

               painter.drawPixmap( p1, builder.Draw( lines, x_grapf_range, y_grapf_range, x_step, y_step, x_msg, y_msg, true ) );

               painter.restore();
           }, 1, 480  );
   };

   res = DrawGrafs( 0, "Зависимость мощности насоса от давления", "Давление, бар", "Мощность, кВт");
   res = DrawGrafs( 1, "Зависимость КПД насоса от давления", "Давление, бар", "КПД, %");
   res = DrawGrafs( 2, "Зависимость подачи насоса от давления (секция 1)", "Давление, бар", "Подача, л/мин");
   res = DrawGrafs( 4, "Зависимость коэффициента подачи насоса от давления (секция 1)", "Давление, бар", "Коэффициент подачи");

   if ( params->SectionsCount() == 2 )
   {
      res = DrawGrafs( 3, "Зависимость подачи насоса от давления (секция 2)", "Давление, бар", "Подача, л/мин");
      res = DrawGrafs( 5, "Зависимость коэффициента подачи насоса от давления (секция 2)", "Давление, бар", "Коэффициент подачи");
   }

   if ( res )
      free_rect.setHeight(0);
   return res;
}


}//namespace pump
}//namespace test

