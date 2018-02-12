#include "pump_test_10.h"
#include <QJsonObject>
#include "../test_params_pumps.h"
#include "../../../../mylib/Widgets/GraphBuilder/graph_builder.h"
namespace test
{
namespace pump
{

namespace
{
QJsonArray ToJson( PumpTest10::Source const& in_src )
{
    QJsonArray source;
    for ( auto it = in_src.begin(), end = in_src.end(); it != end; ++it )
    {
        double key = it->first;
        PumpTest10::DataSet const& data = it->second;
        QJsonArray data_set;
        for ( auto it2 = data.begin(), end2 = data.end(); it2!=end2; ++it2)
        {
            PumpTest10::ArrData const& lnk = *it2;
            QJsonObject item;
            item.insert( "x",    lnk.first );
            item.insert( "y",    lnk.second );
            data_set.insert( data_set.end(), item );
        }
        QJsonObject src;
        src.insert("key", key);
        src.insert("data_set", data_set);
        source.insert( source.end(), src );
    }

    return source;
}
PumpTest10::Source FromJson( QJsonArray arr )
{
    PumpTest10::Source res;
    foreach (QJsonValue const& v, arr)
    {
        QJsonObject src = v.toObject();
        double key = src.value("key").toDouble();
        PumpTest10::DataSet ds;
        QJsonArray data_set = src.value("data_set").toArray();
        foreach ( QJsonValue const& v_item, data_set )
        {
            QJsonObject item = v_item.toObject();
            PumpTest10::ArrData it;
            it.first =       item.value("x").toDouble();
            it.second = item.value("y").toDouble();
            ds.push_back( it );
        }
        res.insert( PumpTest10::SourceItem( key, ds ) );
    }
    return std::move( res );
}

ff0x::NoAxisGraphBuilder::LinePoints Process ( PumpTest10::DataSet const& src, QPointF& x_range, QPointF& y_range )
{
    ff0x::NoAxisGraphBuilder::LinePoints result;

    for ( size_t i = 0; i < src.size(); ++i )
    {
        double const x = fabs( src[i].first );
        double const y = fabs( src[i].second );

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
      mData.resize(4); //4 типа графиков

      //поиск данных теста
      PumpTest10::Source power;
      PumpTest10::Source expenditure;
      PumpTest10::Source exp_coeff;
      PumpTest10::Source kpd;

      foreach (QJsonValue const& val, test::ReadFromFile(compare_width).value("Results").toArray())
      {
         auto obj = val.toObject();
         if ( obj.value("id").toInt() == test->mId )
         {
            auto data = obj.value("data").toObject();
             QJsonArray a0 = data.value("mPower").toArray();
             power = FromJson( a0 );
             QJsonArray a1 = data.value("mExpenditure").toArray();
             expenditure = FromJson( a1 );
             QJsonArray a2 = data.value("mExpCoeff").toArray();
             exp_coeff = FromJson( a2 );
             QJsonArray a3 = data.value("mKPD").toArray();
             kpd = FromJson( a3 );
         }
      }

      for( auto it = power.begin(), end = power.end(); it !=end; ++it )
      {
         Data &d0 = mData[0][it->first];
         d0.templdate = Process( it->second , d0.x_range, d0.y_range );
      }
      for( auto it = expenditure.begin(), end = expenditure.end(); it !=end; ++it )
      {
         Data &d1 = mData[1][it->first];
         d1.templdate = Process( it->second , d1.x_range, d1.y_range );
      }
      for( auto it = exp_coeff.begin(), end = exp_coeff.end(); it !=end; ++it )
      {
         Data &d2 = mData[2][it->first];
         d2.templdate = Process( it->second , d2.x_range, d2.y_range );
      }
      for( auto it = kpd.begin(), end = kpd.end(); it !=end; ++it )
      {
         Data &d3 = mData[3][it->first];
         d3.templdate = Process( it->second , d3.x_range, d3.y_range );
      }

      for( auto it = test->mPower.begin(), end = test->mPower.end(); it !=end; ++it )
      {
         QPointF x_range;
         QPointF y_range;

         Data &d0 = mData[0][it->first];
         d0.data = Process( it->second , x_range, y_range );
         d0.x_range = ff0x::MergeRanges( x_range, d0.x_range );
         d0.y_range = ff0x::MergeRanges( y_range, d0.y_range );
      }
      for( auto it = test->mExpenditure.begin(), end = test->mExpenditure.end(); it !=end; ++it )
      {
         QPointF x_range;
         QPointF y_range;

         x_range = QPointF();
         y_range = QPointF();
         Data &d1 = mData[1][it->first];
         d1.data = Process( it->second , x_range, y_range );
         d1.x_range = ff0x::MergeRanges( x_range, d1.x_range );
         d1.y_range = ff0x::MergeRanges( y_range, d1.y_range );
      }
      for( auto it = test->mExpCoeff.begin(), end = test->mExpCoeff.end(); it !=end; ++it )
      {
         QPointF x_range;
         QPointF y_range;

         x_range = QPointF();
         y_range = QPointF();
         Data &d2 = mData[2][it->first];
         d2.data = Process( it->second , x_range, y_range );
         d2.x_range = ff0x::MergeRanges( x_range, d2.x_range );
         d2.y_range = ff0x::MergeRanges( y_range, d2.y_range );
      }
      for( auto it = test->mKPD.begin(), end = test->mKPD.end(); it !=end; ++it )
      {
         QPointF x_range;
         QPointF y_range;

         x_range = QPointF();
         y_range = QPointF();
         Data &d3 = mData[3][it->first];
         d3.data = Process( it->second , x_range, y_range );
         d3.x_range = ff0x::MergeRanges( x_range, d3.x_range );
         d3.y_range = ff0x::MergeRanges( y_range, d3.y_range );
      }

   }

    struct Data
    {
       ff0x::NoAxisGraphBuilder::LinePoints data;
       ff0x::NoAxisGraphBuilder::LinePoints templdate;

       QPointF x_range;
       QPointF y_range;
    };
    typedef std::map< double, Data > Grapfs; //массивы графиков сортированняе по частоте вращения насоса
    typedef std::vector< Grapfs > DataSet; //вектор графиков по типам

    DataSet mData;
};

PumpTest10::PumpTest10():
    test::pump::Test( "Функциональные зависимости", 49 ),
    mResult(false)
{
}

PumpTest10::~PumpTest10()
{}

bool PumpTest10::Run()
{
    Start();
    Wait( mBits.OP49_Work, mBits.OP49_End );
    if ( IsStopped() )
        return false;
    return Success();
}

QJsonObject PumpTest10::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mResult",            mResult );
    obj.insert("mPower",    ToJson(mPower) );
    obj.insert("mExpenditure",    ToJson(mExpenditure) );
    obj.insert("mExpCoeff",    ToJson(mExpCoeff) );
    obj.insert("mKPD",    ToJson(mKPD) );

    return obj;
}
bool PumpTest10::Deserialize( QJsonObject const& obj )
{
    mResult = obj.value("mResult").toBool();
    mPower = FromJson( obj.value("mPower").toArray() );
    mExpenditure = FromJson( obj.value("mExpenditure").toArray() );
    mExpCoeff = FromJson( obj.value("mExpCoeff").toArray() );
    mKPD = FromJson( obj.value("mKPD").toArray() );
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
     drw.DrawRowLeft( rect, text_font, Qt::black, "1) Подачи насоса от давления при различной частоте вращения насоса;" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "2) Мощности насоса от давления при различной частоте вращения насоса;" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "3) Коэффициент подачи от давления при различной частоте вращения насоса;" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "4) КПД от давления при различной частоте вращения насоса;" );
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

   auto DrawGrafs = [this, &num, &free_rect, &text_font, &painter, &text_font, &drw, &metrix]
         ( Grafs const& grapfs, QString const& title, QString const& x_msg, QString const& y_msg )
   {
      return DrawLine( num, free_rect, text_font,
           [ this, &painter, &text_font, &drw, &metrix, &grapfs, &title, &x_msg, &y_msg ]( QRect const& rect )
           {
               painter.save();

               QFont f = text_font;
               f.setPointSize( 12 );
               int w = (rect.width())*0.98;
               int h = (rect.height() - metrix.height())*0.98;

               ff0x::NoAxisGraphBuilder builder ( w, h, f );
               ff0x::NoAxisGraphBuilder::GraphDataLine lines;
               QPointF x_range;
               QPointF y_range;
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

                  double key = it->first;
                  lines.push_back( ff0x::NoAxisGraphBuilder::Line(data.data, ff0x::NoAxisGraphBuilder::LabelInfo( "Испытуемый аппарат " + test::ToString(key), Qt::blue ) ) );
                  if ( !data.templdate.isEmpty() )
                     lines.push_back( ff0x::NoAxisGraphBuilder::Line(data.templdate, ff0x::NoAxisGraphBuilder::LabelInfo( "Предыдущий результат" + test::ToString(key), Qt::gray ) ) );
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

   res = DrawGrafs( mGrapfs->mData[0], "Зависимость мощьности насоса от давления", "Давление, бар", "Мощность, кВт");
   res = DrawGrafs( mGrapfs->mData[1], "Зависимость подачи насоса от давления", "Давление, бар", "Подача, л/мин");
   res = DrawGrafs( mGrapfs->mData[2], "Зависимость коэффициента подачи насоса от давления", "Давление, бар", "Коэффициент подачи");
   res = DrawGrafs( mGrapfs->mData[3], "Зависимость КПД насоса от давления", "Давление, бар", "КПД, %");

   if ( res )
      free_rect.setHeight(0);
   return res;
}


}//namespace pump
}//namespace test

