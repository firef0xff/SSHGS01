#include "pump_test_6.h"
#include <QJsonObject>
#include "../test_params_pumps.h"

namespace test
{
namespace pump
{

PumpTest6::PumpTest6():
    test::pump::Test( "Определение коэффициента подачи", 45 ),
    mData(0.0)
{}

bool PumpTest6::Run()
{
    Start();
    Wait( mBits.OP45_Work, mBits.OP45_End );
    if ( IsStopped() )
        return false;

    OilTemp = round( mSensors.BT2 *100)/100;
    mData = mBits.OP45_K_Podacha;
    return Success();
}

QJsonObject PumpTest6::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mData",              mData );

    return obj;
}
bool PumpTest6::Deserialize( QJsonObject const& obj )
{
    mData = obj.value("mData").toDouble();
    Test::Deserialize( obj );
    return true;
}

bool PumpTest6::Success() const
{
    return true;
}
QString PumpTest6::RepRes() const
{
   return test::ToString(mData);
}
QString PumpTest6::RepName() const
{
   return "Коэффициент подачи насоса";
}

bool PumpTest6::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
      drw.DrawRowCenter( rect, header_font, Qt::black, "6."+mName );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
      drw.DrawRowLeft( r, text_font, Qt::black, "Цель данного испытания - определение коэффициента подачи насоса при" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "номинальной частоте вращения насоса." );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});

   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &result_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, result_font, Qt::black, "Параметры во время испытаний:" );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font, &params ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Частота вращения, об/мин"), Qt::red, "N/A" );
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
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Коэффициент подачи насоса: ",   Qt::red, test::ToString(mData));
   }, 1.5 );

   if ( res )
      free_rect.setHeight(0);
   return res;
}


}//namespace pump
}//namespace test

//В случае, не возможности выйти на заданное давление, по причине превышения допустимой мощности стенда,
//уменьшаем давление до значения, обусловленного мощностью стенда. При этом сохраняя необходимую частоту вращения.
//И в отчете указать, что испытания проводились не при номинальном давлении (указать при каком давлении).
//В случае не возможности выйти на заданную частоту вращения вала насоса, принять обороты допустимые для данного стенда (от 200 до 2900 об/мин).
// И об этом сообщается оператору.
