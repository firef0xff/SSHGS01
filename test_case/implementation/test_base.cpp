#include "test_base.h"
#include "../tests.h"
#include "../../settings/settings.h"
#include <thread>
#include "test_params_servo.h"
namespace test
{

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
            mCommand.Write();
            Log( "Испытание прервано" );
            return;
        }
    }
    TestingTime = StartTime.elapsed()/1000;

//    Exceptions.Read();
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
    m21Results( cpu::CpuMemory::Instance().DB12 ),
    m12Results( cpu::CpuMemory::Instance().DB3 ),
    m22Results( cpu::CpuMemory::Instance().DB13 ),
    m13Results( cpu::CpuMemory::Instance().DB9 ),
    m23Results( cpu::CpuMemory::Instance().DB19 ),
    m14Result1( cpu::CpuMemory::Instance().DB10 ),
    m14Result2( cpu::CpuMemory::Instance().DB23 ),
    m24Result1( cpu::CpuMemory::Instance().DB20 ),
    m24Result2( cpu::CpuMemory::Instance().DB24 ),
    m15Result( cpu::CpuMemory::Instance().DB11 ),
    m25Result( cpu::CpuMemory::Instance().DB21 ),
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



}//namespace test

