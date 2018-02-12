#include "pump_test_2.h"
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include "../test_params_pumps.h"
#include <functional>

namespace test
{
namespace pump
{

PumpTest2::PumpTest2():
    test::pump::Test( "Испытание функционирования при максимальном давлении", 41 ),
    mResult(false),
    mContol( cpu::CpuMemory::Instance().M2 )
{}

bool PumpTest2::Run()
{
   mIsSet = false;
   mResult = false;
   mInvalidExpenditure1 = false;
   mInvalidExpenditure2 = false;
   mMaxExp1 = 0.0;
   mMinExp1 = 0.0;
   mMaxExp2 = 0.0;
   mMinExp2 = 0.0;

   mContol.Reset();

   Start();
   Wait( mBits.OP41_Work, mBits.OP41_End );
   if ( IsStopped() )
      return false;
   return Success();
}

void PumpTest2::UpdateData()
{
   Test::UpdateData();
   mWarns.Read();
   mInvalidExpenditure1 = mWarns.OP40_Qr_NO_Dopusk1;
   mInvalidExpenditure2 = mWarns.OP40_Qr_NO_Dopusk2;

   mMaxExp1 = mBits.MaxExp1;
   mMinExp1 = mBits.MinExp1;
   mMaxExp2 = mBits.MaxExp2;
   mMinExp2 = mBits.MinExp2;

   if ( mBits.OP40_control && !mIsSet )
   {
      std::mutex mutex;
      std::unique_lock< std::mutex > lock( mutex );
      Launcher( std::bind( &PumpTest2::Question, this ) );

      mCondVar.wait( lock );
      mIsSet = true;

      if ( mResult )
         mContol.Pump_Continue = true;
      else
         mContol.Pump_Fail = true;
      mContol.Write();
   }
}
void PumpTest2::Question()
{
    QMessageBox msg;
    msg.setWindowTitle( "Визуальный контроль" );
    msg.setText( "В системе отсутствуют удары, стуки, повышенная вибрация,\n"
                 "резкий шум, повышенный нагрев,\n"
                 "отсутствуют каплеобразования из под крышек, пробок,\n"
                 "фланцев, через стыки корпусных деталей и т.д." );
    QPushButton *no = msg.addButton("Нет", QMessageBox::NoRole );
    QPushButton *yes = msg.addButton("Да", QMessageBox::YesRole );
    msg.setModal( true );
    no->setDefault( false );
    yes->setDefault( false );
    msg.exec();
    mResult = msg.clickedButton() == yes;
    mCondVar.notify_all();
}


QJsonObject PumpTest2::Serialise() const
{
    QJsonObject obj = Test::Serialise();
    obj.insert("mResult",            mResult );
    obj.insert("InvalidExpenditure1",            mInvalidExpenditure1 );
    obj.insert("InvalidExpenditure2",            mInvalidExpenditure2 );

    obj.insert("mMaxExp1",            mMaxExp1 );
    obj.insert("mMinExp1",            mMinExp1 );
    obj.insert("mMaxExp2",            mMaxExp2 );
    obj.insert("mMinExp2",            mMinExp2 );

    return obj;
}
bool PumpTest2::Deserialize( QJsonObject const& obj )
{
    mResult = obj.value("mResult").toBool();
    mInvalidExpenditure1 = obj.value("InvalidExpenditure1").toBool();
    mInvalidExpenditure2 = obj.value("InvalidExpenditure2").toBool();

    mMaxExp1 = obj.value("mMaxExp1").toDouble();
    mMinExp1 = obj.value("mMinExp1").toDouble();
    mMaxExp2 = obj.value("mMaxExp2").toDouble();
    mMinExp2 = obj.value("mMinExp2").toDouble();

    Test::Deserialize( obj );
    return true;
}

bool PumpTest2::Success() const
{
    return mResult;
}
QString PumpTest2::RepRes() const
{
   return Success()? QString("Да ") : QString(" Нет ");
}
QString PumpTest2::RepName() const
{
   return "Испытание функционирования при максимальном\nзначении давления";
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
      drw.DrawRowLeft( rect, text_font,   Qt::black, "В системе отсутствуют удары, стуки, повышенная вибрация, резкий шум,");
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font, params ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font,   Qt::black, "повышенный нагрев, отсутствуют каплеобразования из под крышек, пробок,");
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font, params ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font,   Qt::black, "фланцев, через стыки корпусных деталей и т.д.: ",
                       Qt::red, RepRes());
   }, 1.5 );

   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font, params ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font, Qt::black, "Расход в секции 1 ",  Qt::red, mInvalidExpenditure1 ? "не в допуске" :"в допуске");
   }, 1.5 );
   res = DrawLine( num, free_rect, text_font,
   [ this, &drw, &text_font, params ]( QRect const& rect )
   {
      drw.DrawRowLeft( rect, text_font,   Qt::black, "Допуск для секции 1: " ,
                       Qt::red, test::ToString( mMinExp1 ) + " - " + test::ToString( mMaxExp1 ) );
   }, 1.5 );

   if ( params->SectionsCount() == 2 )
   {
      res = DrawLine( num, free_rect, text_font,
      [ this, &drw, &text_font, params ]( QRect const& rect )
      {
         drw.DrawRowLeft( rect, text_font, Qt::black, "Расход в секции 2 ",  Qt::red, mInvalidExpenditure2 ? "не в допуске" :"в допуске");
      }, 1.5 );
      res = DrawLine( num, free_rect, text_font,
      [ this, &drw, &text_font, params ]( QRect const& rect )
      {
         drw.DrawRowLeft( rect, text_font,   Qt::black, "Допуск для секции 2: " ,
                          Qt::red, test::ToString( mMinExp2 ) + " - " + test::ToString( mMaxExp2 ) );
      }, 1.5 );
   }

   if ( res )
      free_rect.setHeight(0);
   return res;
}

}//namespace pump
}//namespace test

//В случае не возможности выйти на заданное давление по причине превышения допустимой мощности стенда.
//Уменьшаем обороты насоса до значения, обусловленного мощностью стенда, при котором возможно создать необходимое давление.
//В случае не возможности выйти на заданную частоту вращения вала насоса, принять обороты допустимые для данного стенда (от 200 до 2900 об/мин).
//И об этом сообщается оператору.
