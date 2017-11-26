#include "pump_test_4.h"
#include <QJsonObject>
#include "../test_params_pumps.h"

namespace test
{
namespace pump
{

PumpTest4::PumpTest4():
    test::pump::Test( "Расчет КПД", 43 ),
    mResult(false),
    mData(0.0)
{}

bool PumpTest4::Run()
{
    Start();
//    Wait( mBits.op32_ok, mBits.op32_end );
    if ( IsStopped() )
        return false;

//    OilTemp = round( mTemperature.T_oil *100)/100;

    return Success();
}

QJsonObject PumpTest4::Serialise() const
{
    QJsonObject obj;
    obj.insert("mResult",            mResult );
    obj.insert("mData",              mData );

    return obj;
}
bool PumpTest4::Deserialize( QJsonObject const& obj )
{
    mResult = obj.value("mResult").toBool();
    mData = obj.value("mData").toDouble();
    return true;
}

bool PumpTest4::Success() const
{
    return mResult;
}
bool PumpTest4::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
      drw.DrawRowCenter( rect, header_font, Qt::black, "4."+mName );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
      drw.DrawRowLeft( r, text_font, Qt::black, "Данное испытание предназначено для определения КПД насоса." );
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
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении тспытаний, бар"), Qt::red, "N/A" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Подача насоса при проведении испытания, л/мин"), Qt::red, "N/A" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Крутящий момент при проведении испытания, Нм"), Qt::red, "N/A" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Частота вращения при проведении испытания, об/мин"), Qt::red, "N/A" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font, &params ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Длительность испытания, сек"), Qt::red, "N/A" );
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
      drw.DrawRowLeft( rect, text_font,   Qt::black, "КПД насоса, %: ",   Qt::red, test::ToString(mData));
   }, 1.5 );

   if ( res )
      free_rect.setHeight(0);
   return res;
}


}//namespace pump
}//namespace test

