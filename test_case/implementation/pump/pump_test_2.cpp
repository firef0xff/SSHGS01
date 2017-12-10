#include "pump_test_2.h"
#include <QJsonObject>
#include "../test_params_pumps.h"

namespace test
{
namespace pump
{

PumpTest2::PumpTest2():
    test::pump::Test( "Испытание функционирования при максимальном давлении", 41 ),
    mResult(false)
{}

bool PumpTest2::Run()
{
    Start();
//    Wait( mBits.op32_ok, mBits.op32_end );
    if ( IsStopped() )
        return false;

//    OilTemp = round( mTemperature.T_oil *100)/100;

    return Success();
}
QString PumpTest2::RepRes()
{
   return Success()? QString(" функционирует ") : QString(" не работает ");
}
QString PumpTest2::RepName()
{
   return "Испытание функционирования при максимальном\nзначении давления";
}
QJsonObject PumpTest2::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mResult",            mResult );

    return obj;
}
bool PumpTest2::Deserialize( QJsonObject const& obj )
{
    mResult = obj.value("mResult").toBool();
    Test::Deserialize( obj );
    return true;
}

bool PumpTest2::Success() const
{
    return mResult;
}
bool PumpTest2::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
      drw.DrawRowCenter( rect, header_font, Qt::black, "2."+mName );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
      drw.DrawRowLeft( r, text_font, Qt::black, "Данное испытание предназначено для проверки функционирования при" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font, Qt::black,
      "максимальном давлении." );
   }, 1.5 );


   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});

   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &result_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, result_font, Qt::black, "Параметры во время испытаний:" );
   }, 1.5 );

   auto Pressure = []( double n, double m )
   {
      double r = n*1.25;
      return r <= m ? r : m;
   };

   QString press = test::ToString( Pressure( params->PressureNom1(), params->PressureMax1() ) );
   if ( params->SectionsCount() > 1 )
      press += ", " + test::ToString( Pressure( params->PressureNom2(), params->PressureMax2() ) );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font,&params, &press ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление при проведении испытаний, бар"), Qt::red, press );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font, &params ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Длительность испытания, мин"), Qt::red, test::ToString(TestingTime) );
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
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Гидронасос ",
                                     Qt::red, RepRes());
   }, 1.5 );

   if ( res )
      free_rect.setHeight(0);
   return res;
}

}//namespace pump
}//namespace test

//В случае не возможности выйти на заданное давление по причине превышения допустимой мощности стенда.  Уменьшаем обороты насоса до значения, обусловленного мощностью стенда, при котором возможно создать необходимое давление.
//В случае не возможности выйти на заданную частоту вращения вала насоса, принять обороты допустимые для данного стенда (от 200 до 2900 об/мин).
//И об этом сообщается оператору.
