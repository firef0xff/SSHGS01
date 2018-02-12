#include "pump_test_9.h"
#include <QJsonObject>
#include "../test_params_pumps.h"

namespace test
{
namespace pump
{

PumpTest9::PumpTest9():
    test::pump::Test( "Расход в дренаже", 48 ),
    mExp(0.0)
{}

bool PumpTest9::Run()
{
    Start();
    Wait( mBits.OP48_Work, mBits.OP48_End );
    if ( IsStopped() )
        return false;

    mExp = mBits.OP48_Q_Drenag;

    return Success();
}

QJsonObject PumpTest9::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mExp",               mExp );

    return obj;
}
bool PumpTest9::Deserialize( QJsonObject const& obj )
{
    mExp = obj.value("mExp").toDouble();
    Test::Deserialize( obj );
    return true;
}

bool PumpTest9::Success() const
{
    return true;
}
QString PumpTest9::RepRes() const
{
   QString res = "\n";
   res += test::ToString( mExp );
   res += " л/мин";
   return res;
}
QString PumpTest9::RepName() const
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
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Температура масла во время испытаний, ˚С"), Qt::red, test::ToString(OilTemp) );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Расход в секции 1, л/мин"), Qt::red, test::ToString(mExp1) );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление в секции 1, Бар"), Qt::red, test::ToString(mPress1) );
   }, 1.5 );
   if ( params->SectionsCount() == 2 )
   {
      res = DrawLine( num, free_rect, text_font,
      [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
      {
        drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Расход в секции 2, л/мин"), Qt::red, test::ToString(mExp2) );
      }, 1.5 );
      res = DrawLine( num, free_rect, text_font,
      [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
      {
        drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление в секции 2, Бар"), Qt::red, test::ToString(mPress2) );
      }, 1.5 );
   }
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
   {
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Частота вращения, об/мин"), Qt::red, test::ToString(mFrequency) );
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
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Расход в дренаже ",
                                          Qt::red, mExp >= 0.5 ? test::ToString( mExp ) + " л/мин" : QString(" меньше 0.5 л/мин"),
                                          Qt::black, "заявленным данным");
   }, 1.5 );

   if ( res )
      free_rect.setHeight(0);
   return res;
}


}//namespace pump
}//namespace test

//В случае не возможности выйти на заданное давление или частоту вращения по причине превышения допустимой мощности стенда.
//То данное испытание проводить нельзя.
//Если нет значения в водимых данных, то испытания не проводим.
//И об этом сообщается оператору.
