#include "pump_test_3.h"
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include "../test_params_pumps.h"
#include <functional>

namespace test
{
namespace pump
{

PumpTest3::PumpTest3():
    test::pump::Test( "Наружная герметичность", 42 ),
    mResult(false)
{}

bool PumpTest3::Run()
{
    Start();
    Wait( mBits.OP42_Work, mBits.OP42_End );
    if ( IsStopped() )
        return false;

    std::mutex mutex;
    std::unique_lock< std::mutex > lock( mutex );
    Launcher( std::bind( &PumpTest3::Question, this ) );

    mCondVar.wait( lock );

    return Success();
}

QJsonObject PumpTest3::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mResult",            mResult );

    return obj;
}
bool PumpTest3::Deserialize( QJsonObject const& obj )
{
    mResult = obj.value("mResult").toBool();
    Test::Deserialize( obj );
    return true;
}

bool PumpTest3::Success() const
{
    return mResult;
}
QString PumpTest3::RepRes() const
{
   return "течь" + ( Success()? QString(" не обнаружена ") : QString(" обнаружена ") );
}
QString PumpTest3::RepName() const
{
   return "Наружная герметичность";
}
bool PumpTest3::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
      drw.DrawRowCenter( rect, header_font, Qt::black, "3."+mName );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
      drw.DrawRowLeft( r, text_font, Qt::black, "Данное испытание проходит аналогично испытанию на прочность, только" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font, Qt::black,
      "давление должно быть не более максимального и результатом испытаний будет" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font, Qt::black,
      "визуальный осмотр." );
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
     drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление в секции 1, Бар"), Qt::red, test::ToString(mPress1) );
   }, 1.5 );
   if ( params->SectionsCount() == 2 )
   {
      res = DrawLine( num, free_rect, text_font,
      [ this, &drw, &FillToSize, &text_font ]( QRect const& rect )
      {
        drw.DrawRowLeft( rect, text_font, Qt::black, FillToSize("Давление в секции 2, Бар"), Qt::red, test::ToString(mPress2) );
      }, 1.5 );
   }

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
      drw.DrawRowLeft( rect, text_font,   Qt::black, "При испытании наружной герметичности ",
                                     Qt::red, RepRes());
   }, 1.5 );

   if ( res )
      free_rect.setHeight(0);
   return res;
}
void PumpTest3::Question()
{
    QMessageBox msg;
    msg.setWindowTitle( "Визуальный контроль" );
    msg.setText( "Обнаружена ли течь или потение наружных\nповерхностей на испытуемом гидронасосе" );
    QPushButton *no = msg.addButton("Нет", QMessageBox::NoRole );
    QPushButton *yes = msg.addButton("Да", QMessageBox::YesRole );
    msg.setModal( true );
    no->setDefault( false );
    yes->setDefault( false );
    msg.exec();
    mResult = msg.clickedButton() == no;
    mCondVar.notify_all();
}


}//namespace pump
}//namespace test

//В случае не возможности выйти на заданное давление по причине превышения допустимой мощности стенда.
//Уменьшаем обороты насоса до значения, обусловленного мощностью стенда, при котором возможно создать необходимое давление.
//В случае не возможности выйти на заданную частоту вращения вала насоса, принять обороты допустимые для данного стенда (от 200 до 2900 об/мин).
//И об этом сообщается оператору.
