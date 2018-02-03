#include "test_base.h"
#include "../tests.h"
#include "../../settings/settings.h"
#include <thread>
#include "test_params_servo.h"
#include <QMessageBox>
#include <QPushButton>
#include <condition_variable>
namespace test
{

QString db40_errs()
{
   auto& errs = cpu::CpuMemory::Instance().DB40;
   errs.Read();

   QString str_errs;
//    if (errs.sl1)
//        str_errs += "уровень масла в норме\n";
   if (errs.sl2)
       str_errs += "пониженный уровень масла\n";
   if (errs.sl3)
       str_errs += "аварийный уровень масла\n";
   if (errs.sl4)
       str_errs += "верхний уровень перелив\n";
//    if (errs.sl5)
//        str_errs += "нижний уровень перелив\n";
   if (errs.sp1)
       str_errs += "фильтр всасывающий М1 загрязнен\n";
   if (errs.sp2)
       str_errs += "фильтр всасывающий М2 загрязнен\n";
   if (errs.sp3)
       str_errs += "фильтр всасывающий М3 загрязнен\n";
   if (errs.sp4)
       str_errs += "фильтр всасывающий М3 загрязнен\n";
   if (errs.sp5)
       str_errs += "фильтр всасывающий М4 загрязнен\n";
   if (errs.sp6)
       str_errs += "фильтр всасывающий М5 загрязнен\n";
   if (errs.sp7)
       str_errs += "фильтр тонкой очистки М1 загрязнен\n";
   if (errs.sp8)
       str_errs += "фильтр грубой очистки М1 загрязнен\n";
   if (errs.sp9)
       str_errs += "фильтр тонкой очистки М2 загрязнен\n";
   if (errs.sp10)
       str_errs += "фильтр грубой очистки М2 загрязнен\n";
   if (errs.sp11)
       str_errs += "фильтр тонкой очистки М4 загрязнен\n";
   if (errs.sp12)
       str_errs += "фильтр грубой очистки М3 загрязнен\n";
   if (errs.sp13)
       str_errs += "фильтр грубой очистки М3 загрязнен\n";
   if (errs.sp14)
       str_errs += "фильтр контура охлаждения загрязнен\n";
   if (errs.sp15)
       str_errs += "фильтр тонкой очистки М12 загрязнен\n";
   if (errs.sq1)
       str_errs += "кран М1 закрыт\n";
   if (errs.sq3)
       str_errs += "кран М2 закрыт\n";
   if (errs.sq5)
       str_errs += "кран М3 закрыт\n";
   if (errs.sq7)
       str_errs += "кран М3 закрыт\n";
   if (errs.sq9)
       str_errs += "кран М4 закрыт\n";
   if (errs.sq11)
       str_errs += "кран М5 закрыт\n";
   if (errs.sq13)
       str_errs += "кран дренажа М1 закрыт\n";
   if (errs.sq14)
       str_errs += "кран дренажа М2 закрыт\n";
   if (errs.sq15)
       str_errs += "кран дренажа М4 закрыт\n";
   if (errs.fault_m1)
       str_errs += "нет включения М1\n";
   if (errs.fault_m2)
       str_errs += "нет включения М2\n";
   if (errs.fault_m3)
       str_errs += "нет включения М3\n";
   if (errs.fault_m4)
       str_errs += "нет включения М4\n";
   if (errs.fault_m5)
       str_errs += "нет включения М5\n";
   if (errs.fault_m6)
       str_errs += "нет включения М6\n";
   if (errs.fault_m8)
       str_errs += "нет включения М8\n";
   if (errs.fault_m10)
       str_errs += "нет включения М10\n";
   if (errs.fault_m12)
       str_errs += "нет включения М12\n";
   if (errs.fault_phasing)
       str_errs += "не правильная фазировка\n";
   if (errs.p_no)
       str_errs += "давление в системе не установилось\n";
   if (errs.p_upr_no)
       str_errs += "давление в канале Х не установилось\n";
   if (errs.q_no)
       str_errs += "расход не установился\n";
   if (errs.v_no)
       str_errs += "Напряжение не в допуске\n";
   if (errs.fault_p_op12)
       str_errs += "Аварийное давление\n";
   if (errs.fault_p_op22)
       str_errs += "Аварийное давление\n";

   return str_errs;
}
QString db73_errs()
{
   auto& errs = cpu::CpuMemory::Instance().DB73;
   errs.Read();

   QString str_errs;
   if (errs.SP16_warning) str_errs+="загрязнен фильтр в контуре А\n";
   if (errs.SP17_warning) str_errs+="загрязнен фильтр в контуре В\n";
   if (errs.SP18_warning) str_errs+="загрязнен фильтр в контуре С\n";
   if (errs.SP19_warning) str_errs+="загрязнен фильтр линии упр.\n";
   if (errs.SP20_warning) str_errs+="загрязнен фильтр линии утечек\n";
   if (errs.SP21_warning) str_errs+="загрязнен фильтр контура охлаждения\n";
   if (errs.SP22_warning) str_errs+="загрязнен фильтр линии слива\n";
   if (errs.SP23_warning) str_errs+="нет давления в пн/системе\n";
   if (errs.SP24_warning) str_errs+="загрязнен фильтр дренажа\n";
   if (errs.SP25_warning) str_errs+="загрязнен фильтр контрол-го дренажа\n";
   if (errs.SQ16_warning) str_errs+="кран на подкл-ние всасывающей линии - закрыт\n";
   if (errs.SQ17_warning) str_errs+="кран на подкл-ние дренажной линии - закрыт\n";
   if (errs.SQ18_warning) str_errs+="кран на отключение всасывающей линии - закрыт\n";
   if (errs.SQ19_warning) str_errs+="кран закрытие дренажной линии - закрыт\n";
   if (errs.SQ20_warning) str_errs+="нет закрытия защитного кожуха\n";
   if (errs.SQ21_warning) str_errs+="нет закрытия кожуха муфты\n";
   if (errs.Q_NO_Istablished) str_errs+="Расход в регуляторе не установился\n";
   if (errs.P_NO_Istablished) str_errs+="Давление в регуляторе не установилось\n";
   if (errs.OP40_Q_Nul) str_errs+="Расход в нуле\n";
   if (errs.OP40_P_Nul) str_errs+="Давленине в нуле\n";
   if (errs.OP40_Qr_NO_Dopusk) str_errs+="Расход рабочий не в допуске (функц-ние)\n";
   if (errs.OP41_Qr_NO_Dopusk) str_errs+="Расход рабочий не в допуске (прочность)\n";
   if (errs.OP42_Qr_NO_Dopusk) str_errs+="Расход рабочий не в допуске (гермет-ть)\n";
   if (errs.OP48_Qd_NO_Dopusk) str_errs+="Расход в дренаже не в допуске\n";


   return str_errs;
}

QString ErrMsg()
{
    return db40_errs() + db73_errs();
}

std::condition_variable CondVar;

TestCommonData::TestCommonData( TestCase* test_case, QString const& name, uint8_t number, uint8_t id ):
    test::Test( test_case, name, number, id ),
    OilTemp(0),
    mCommand( cpu::CpuMemory::Instance().DB31 )
{}

QJsonObject TestCommonData::Serialise() const
{
    QJsonObject obj;
    obj.insert("OilTemp", OilTemp );
    obj.insert("TestingTime", TestingTime );

    return obj;
}
bool TestCommonData::Deserialize( QJsonObject const& obj )
{
    OilTemp = obj.value("OilTemp").toDouble();
    TestingTime = obj.value("TestingTime").toInt();
    return true;
}

uint8_t TestCommonData::CommandID()
{
    return mId;
}
void TestCommonData::Start()
{
    mCommand.N_Operation = CommandID();
    mCommand.Start_Oper = true;
    mCommand.Stop_Oper = false;
    mCommand.Nasos_M2 = app::Settings::Instance().MainPupm() == "M2";
    mCommand.OP15_25_Continum = false;
    mCommand.Next_Amp = false;
    mCommand.Write();
    StartTime.start();
}
void TestCommonData::Wait( bool& work, bool& done)
{
    work = false;
    done = false;
    bool started = false;
    while( !done || !started )
    {
        if (work)
            started = true;
        UpdateData();
        if ( IsStopped() )
        {            
            mCommand.Start_Oper = false;
            mCommand.Stop_Oper = true;
            mCommand.Nasos_M2 = app::Settings::Instance().MainPupm() == "M2";
            mCommand.OP15_25_Continum = false;
            mCommand.Next_Amp = false;
            mCommand.Write();
            Log( "Испытание прервано" );
            return;
        }
    }
    TestingTime = StartTime.elapsed()/1000;
    *mStopMarker = !CheckErrors();
}
bool TestCommonData::CheckErrors()
{
//    Exceptions.Read();
    auto& mem = cpu::CpuMemory::Instance().M1;

    QString str_errs = ErrMsg();

    if ( str_errs.isEmpty() )
        return true;

    std::mutex mutex;
    std::unique_lock< std::mutex > lock( mutex );
    Launcher( std::bind( &TestCommonData::ShowErrors, this, str_errs ) );

    CondVar.wait( lock );
    mem.SetKvitir_Osch( true );
    return false;
}
void TestCommonData::ShowErrors( QString const& err )
{
    QMessageBox msg;
    msg.setWindowTitle( "Ошибки тестирования" );
    msg.setText( err.left(200) + "..." );
    msg.addButton( QMessageBox::Ok );
    msg.setModal( true );
    msg.exec();
    CondVar.notify_one();
}

namespace hydro
{

uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::TestCommonData( &HydroTests, name, mTestsCount, id ),
    mResults( cpu::CpuMemory::Instance().DB32 )
{
    ++mTestsCount;
}

void Test::UpdateData()
{
    mResults.Read();
}

}//namespace hydro

namespace servo
{

uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id_board, uint8_t id_reel ):
    test::TestCommonData( &ServoTests, name, mTestsCount, id_board ),
    mTemperature( cpu::CpuMemory::Instance().DB32 ),
    mControlBoardBits( cpu::CpuMemory::Instance().DB34  ),
    mControlReelBits( cpu::CpuMemory::Instance().DB36  ),
    m11Results( cpu::CpuMemory::Instance().DB2 ),
    m12Results( cpu::CpuMemory::Instance().DB3 ),
    m13Results( cpu::CpuMemory::Instance().DB9 ),
    m14Result1( cpu::CpuMemory::Instance().DB10 ),
    m14Result2( cpu::CpuMemory::Instance().DB23 ),
    m15Result( cpu::CpuMemory::Instance().DB11 ),
    m1525Counts( cpu::CpuMemory::Instance().DB60 ),
    mIdReel( id_reel )
{
    ++mTestsCount;
}

void Test::UpdateData()
{
    if (ReelControl())
        mControlReelBits.Read();
    else
        mControlBoardBits.Read();
    mTemperature.Read();
}
uint8_t Test::CommandID()
{    
    return ReelControl() ? mIdReel : mId;
}

bool Test::ReelControl() const
{
    return test::servo::Parameters::Instance().ReelControl() == RC_REEL;
}

}//namespace servo

namespace control_board
{
uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::TestCommonData( &ControlBoardTests, name, mTestsCount, id ),
    mBits( cpu::CpuMemory::Instance().DB39 ),
    mTemperature( cpu::CpuMemory::Instance().DB32 ),
    m31Results( cpu::CpuMemory::Instance().DB22 )
{
    ++mTestsCount;
}
void Test::UpdateData()
{
    mBits.Read();
    mTemperature.Read();
}
}//namespace control_board

namespace hydro_cylinder
{
uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::TestCommonData( &HydroCylinder, name, mTestsCount, id ),
    mBits( cpu::CpuMemory::Instance().DB39 ),
    mTemperature( cpu::CpuMemory::Instance().DB32 )
{
    ++mTestsCount;
}

void Test::UpdateData()
{
    mBits.Read();
    mTemperature.Read();
}

}//namespace hydro_cylinder

namespace pump
{
uint8_t Test::mTestsCount = 1;
Test::Test( QString const& name, uint8_t id ):
    test::TestCommonData( &Pumps, name, mTestsCount, id ),
    mBits( cpu::CpuMemory::Instance().DB72 )
{
    ++mTestsCount;
}

void Test::UpdateData()
{
}


}

}//namespace test

