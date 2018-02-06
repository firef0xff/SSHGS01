#include "pump_test_1.h"
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include "../test_params_pumps.h"
#include <functional>

namespace test
{
namespace pump
{

PumpTest1::PumpTest1():
    test::pump::Test( "Испытание функционирования", 40 ),
    mResult(false),
    mContol( cpu::CpuMemory::Instance().M2 )
{}

bool PumpTest1::Run()
{
    mIsSet = false;
    mResult = false;
    mContol.Reset();
    Start();
    Wait( mBits.OP40_Work, mBits.OP40_End );
    if ( IsStopped() )
        return false;

    OilTemp = round( mSensors.BT2 *100)/100;

    return Success();
}

void PumpTest1::UpdateData()
{
   Test::UpdateData();
   if ( mBits.OP40_control && !mIsSet )
   {
      std::mutex mutex;
      std::unique_lock< std::mutex > lock( mutex );
      Launcher( std::bind( &PumpTest1::Question, this ) );

      mCondVar.wait( lock );
      mIsSet = true;

      if ( mResult )
         mContol.Pump_Continue = true;
      else
         mContol.Pump_Fail = true;
      mContol.Write();
   }
}
void PumpTest1::Question()
{
    QMessageBox msg;
    msg.setWindowTitle( "Визуальный контроль" );
    msg.setText( "В системе отсутствуют: удары, стуки,\n"
                 "повышенная вибрация, резкий шум, повышенный нагрев.\n"
                 "Отсутствуют каплеобразования: из под крышек,\n"
                 "пробок, фланцев, через стыки корпусных деталей и т.д." );
    QPushButton *no = msg.addButton("Нет", QMessageBox::NoRole );
    QPushButton *yes = msg.addButton("Да", QMessageBox::YesRole );
    msg.setModal( true );
    no->setDefault( false );
    yes->setDefault( false );
    msg.exec();
    mResult = msg.clickedButton() == yes;
    mCondVar.notify_all();
}

QJsonObject PumpTest1::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mResult",            mResult );

    return obj;
}
bool PumpTest1::Deserialize( QJsonObject const& obj )
{
    mResult = obj.value("mResult").toBool();
    Test::Deserialize( obj );
    return true;
}

bool PumpTest1::Success() const
{
    return mResult;
}
QString PumpTest1::RepRes() const
{
   return Success()? QString(" функционирует ") : QString(" не работает ");
}
QString PumpTest1::RepName() const
{
   return "Испытание функционирования";
}
bool PumpTest1::Draw(QPainter& painter, QRect &free_rect , const QString &) const
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
      drw.DrawRowCenter( rect, header_font, Qt::black, "1."+mName );
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font, []( QRect const& ){});
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      QRect r(rect.left() + 76, rect.top(), rect.width() - 76, rect.height() );
      drw.DrawRowLeft( r, text_font, Qt::black, "Проверка функционирования проводится в 2 этапа:" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font, Qt::black,
      "I) В начале без нагрузки 2 кратковременных включения по 5 сек., при максимальных" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font, Qt::black,
      "оборотах, и минимальном давлении." );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font, Qt::black,
      "II) Затем плавное повышение давления в течении 10 сек. до номинального давления" );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font, Qt::black,
      "и номинальнной частоты вращения и работа на таких параметрах в течении времени." );
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font, Qt::black,
      "" );
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
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Гидронасос ",
                                     Qt::red, RepRes());
   }, 1.5 );

   if ( res )
      free_rect.setHeight(0);
   return res;
}


}//namespace pump
}//namespace test

/*
ОГРАНИЧЕНИЯ:
В случае, не возможности выйти на заданное давление, по причине превышения допустимой мощности стенда,
уменьшаем давление до значения, обусловленного мощностью стенда. При этом сохраняя необходимую частоту вращения.
В случае не возможности выйти на заданную частоту вращения вала насоса, принять обороты допустимые для данного стенда (от 200 до 2900 об/мин).
И об этом сообщается оператору.
*/
