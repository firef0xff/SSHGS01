#include "pump_test_5.h"
#include <QJsonObject>
#include "../test_params_pumps.h"

namespace test
{
namespace pump
{

PumpTest5::PumpTest5():
    test::pump::Test( "Проверка рабочего объема", 44 ),
    mData1(0.0),
    mData2(0.0)
{}

bool PumpTest5::Run()
{
    Start();
    Wait( mBits.OP44_Work, mBits.OP44_End );
    if ( IsStopped() )
        return false;
    mData1 = mBits.OP44_Work_V_S1;
    mData2 = mBits.OP44_Work_V_S2;
    return Success();
}

QJsonObject PumpTest5::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mData1",              mData1 );
    obj.insert("mData2",              mData2 );

    return obj;
}
bool PumpTest5::Deserialize( QJsonObject const& obj )
{
    mData1 = obj.value("mData1").toDouble();
    mData2 = obj.value("mData2").toDouble();
    Test::Deserialize( obj );
    return true;
}

bool PumpTest5::Success() const
{
    return true;
}
QString PumpTest5::RepRes() const
{
   QString res = test::ToString(mData1)+" см³/об";
   test::pump::Parameters *params = static_cast< test::pump::Parameters * >( CURRENT_PARAMS );
   if ( !params )
      return res;
   if (params->SectionsCount() == 2 )
      res += "\n"+ test::ToString(mData2)+" см³/об";
   return res;
}
QString PumpTest5::RepName() const
{
   QString res = "Рабочий объем секции 1 насоса";
   test::pump::Parameters *params = static_cast< test::pump::Parameters * >( CURRENT_PARAMS );
   if ( !params )
      return res;
   if (params->SectionsCount() == 2 )
   {
      res += "\n";
      res += "Рабочий объем секции 2 насоса";
   }
   return res;
}
bool PumpTest5::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
      drw.DrawRowCenter( rect, header_font, Qt::black, "5."+mName );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
      drw.DrawRowLeft( r, text_font, Qt::black, "Цель данного испытания - определение рабочего объема насоса при двух" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "измеренных частотах вращения насоса." );
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
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font, params ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Измеренный рабочий объем секции 1 насоса, см³/об: ",   Qt::red, test::ToString(mData1));
   }, 1.5 );

   if (params->SectionsCount()==2)
   {
      res = DrawLine( num, free_rect, text_font,
      [ this, &drw, &text_font, params ]( QRect const& rect )
      {
         drw.DrawRowLeft( rect, text_font,   Qt::black, "Измеренный рабочий объем секции 2 насоса, см³/об: ",   Qt::red, test::ToString(mData2));
      }, 1.5 );
   }

   if ( res )
      free_rect.setHeight(0);
   return res;
}


}//namespace pump
}//namespace test

//В случае не возможности выйти на заданную частоту вращения вала насоса, принять обороты допустимые для данного стенда (от 200 до 2900 об/мин).
//И в расчетах этого испытания принимать их как номинальные.
//И об этом сообщается оператору.
