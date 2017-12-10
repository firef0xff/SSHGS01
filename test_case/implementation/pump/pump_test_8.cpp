#include "pump_test_8.h"
#include <QJsonObject>
#include "../test_params_pumps.h"

namespace test
{
namespace pump
{

PumpTest8::PumpTest8():
    test::pump::Test( "Расчет подачи насоса", 47 ),
    mResult(false),
    mExpMin(0.0),
    mExpNom(0.0),
    mExpMax(0.0)
{}

bool PumpTest8::Run()
{
    Start();
//    Wait( mBits.op32_ok, mBits.op32_end );
    if ( IsStopped() )
        return false;

//    OilTemp = round( mTemperature.T_oil *100)/100;

    return Success();
}

QJsonObject PumpTest8::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mResult",            mResult );
    obj.insert("mExpMin",            mExpMin );
    obj.insert("mExpNom",            mExpNom );
    obj.insert("mExpMax",            mExpMax );

    return obj;
}
bool PumpTest8::Deserialize( QJsonObject const& obj )
{
    mResult = obj.value("mResult").toBool();
    mExpMin = obj.value("mExpMin").toDouble();
    mExpNom = obj.value("mExpNom").toDouble();
    mExpMax = obj.value("mExpMax").toDouble();
    Test::Deserialize( obj );
    return true;
}

bool PumpTest8::Success() const
{
    return mResult;
}
QString PumpTest8::RepName()
{
   QString res = "Подача насоса:\n";

   test::pump::Parameters *params = static_cast< test::pump::Parameters * >( CURRENT_PARAMS );
   if ( !params )
      return res;

   res += "- при минимальной частоте насоса (";
   res += test::ToString( params->FrequencyMin() );
   res += " об/мин)\n";

   res += "- при номинальной частоте насоса (";
   res += test::ToString( params->FrequencyNom() );
   res += " об/мин)\n";

   res += "- при максимальной частоте насоса (";
   res += test::ToString( params->FrequencyMax() );
   res += " об/мин)";

   return res;
}
QString PumpTest8::RepRes()
{
   QString res;
   res += test::ToString( mExpMin );
   res += " л/мин\n";
   res += test::ToString( mExpNom );
   res += " л/мин\n";
   res += test::ToString( mExpMax );
   res += " л/мин";
   return res;
}
bool PumpTest8::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
      drw.DrawRowCenter( rect, header_font, Qt::black, "8."+mName );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
      drw.DrawRowLeft( r, text_font, Qt::black, "Цель данного испытания - определение минимальной, номинальной и" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "максимальной производительности насоса, при номинальном давлении." );
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
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Частота вращения минимальная, об/мин"), Qt::red, test::ToString( params->FrequencyMin() ) );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font, &params ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Частота вращения номинальная, об/мин"), Qt::red, test::ToString( params->FrequencyNom() ) );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font, &params ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Частота вращения максимальная, об/мин"), Qt::red, test::ToString( params->FrequencyMax() ) );
   }, 1.5 );

   QString press = test::ToString( params->PressureNom1() );
   if ( params->SectionsCount() > 1 )
      press += ", " + test::ToString( params->PressureNom2() );

   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font, &press ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Рабочее давление, бар"), Qt::red, press );
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
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Подача насоса при минимальной частоте вращения насоса, л/мин: ",   Qt::red, test::ToString(mExpMin));
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font, params ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Подача насоса при номинальной частоте вращения насоса, л/мин: ",   Qt::red, test::ToString(mExpNom));
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font, params ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Подача насоса при максимальной частоте вращения насоса, л/мин: ",   Qt::red, test::ToString(mExpMax));
   }, 1.5 );

   if ( res )
      free_rect.setHeight(0);
   return res;
}


}//namespace pump
}//namespace test

//В случае, не возможности выйти на заданное давление, по причине превышения допустимой мощности стенда,  уменьшаем давление до значения, обусловленного мощностью стенда. При этом сохраняя необходимую частоту вращения.
//В случае не возможности выйти на заданную частоту вращения вала насоса, принять обороты допустимые для данного стенда (от 200 до 2900 об/мин).
//И об этом сообщается оператору.
