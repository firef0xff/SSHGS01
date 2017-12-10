#include "pump_test_9.h"
#include <QJsonObject>
#include "../test_params_pumps.h"

namespace test
{
namespace pump
{

PumpTest9::PumpTest9():
    test::pump::Test( "Расход в дренаже", 48 ),
    mResult(false),
    mExp(0.0)
{}

bool PumpTest9::Run()
{
    Start();
//    Wait( mBits.op32_ok, mBits.op32_end );
    if ( IsStopped() )
        return false;

//    OilTemp = round( mTemperature.T_oil *100)/100;

    return Success();
}

QJsonObject PumpTest9::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mResult",            mResult );
    obj.insert("mExp",               mExp );

    return obj;
}
bool PumpTest9::Deserialize( QJsonObject const& obj )
{
    mResult = obj.value("mResult").toBool();
    mExp = obj.value("mExp").toDouble();
    Test::Deserialize( obj );
    return true;
}

bool PumpTest9::Success() const
{
    return mResult;
}
QString PumpTest9::RepRes()
{
   QString res = "\n";
   res += test::ToString( mExp );
   res += " л/мин\n";
   res += Success()? QString(" соответсвует ") : QString(" не соответсвует ");
   return res;
}
QString PumpTest9::RepName()
{
   return "Расход в дренаже при номинальном режиме";
}
bool PumpTest9::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
      drw.DrawRowCenter( rect, header_font, Qt::black, "9."+mName );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
      drw.DrawRowLeft( r, text_font, Qt::black, "Цель данного испытания - определение расхода в дренажном канале при" );
   }, 1.5 );
   res = DrawLine( num, free_rect, result_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, "при номинальном давлении и расходе, и сравнение его с исходным значением." );
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
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Частота вращения номинальная, об/мин"), Qt::red, test::ToString( params->FrequencyNom() ) );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font, &params ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Подача насоса, л/мин"), Qt::red, "N/A" );
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
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Дренаж ",
                                          Qt::red, Success()? QString(" соответсвует ") : QString(" не соответсвует "),
                                          Qt::black, "заявленным данным");
   }, 1.5 );

   if ( res )
      free_rect.setHeight(0);
   return res;
}


}//namespace pump
}//namespace test

//В случае не возможности выйти на заданное давление или частоту вращения по причине превышения допустимой мощности стенда.  То данное испытание проводить нельзя.
//Если нет значения в водимых данных, то испытания не проводим.
//И об этом сообщается оператору.
