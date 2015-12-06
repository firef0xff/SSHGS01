#include "test_base.h"
#include "../tests.h"
#include "../../settings/settings.h"
#include <thread>
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

void TestCommonData::Start()
{
    mCommand.N_Operation = mId;
    mCommand.Start_Oper = true;
    mCommand.Stop_Oper = false;
    mCommand.Nasos_M2 = app::Settings::Instance().MainPupm() == "M2";
    mCommand.Write();
    StartTime.start();
}
void TestCommonData::Wait( bool& work, bool& done)
{
    work = false;
    done = false;
    while( !done )
    {
        UpdateData();
    }
    TestingTime = StartTime.elapsed()/1000;
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

Test::Test( QString const& name, uint8_t id ):
    test::TestCommonData( &ServoTests, name, mTestsCount, id )
{
    ++mTestsCount;
}
}//namespace servo

namespace control_board
{
uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::TestCommonData( &ControlBoardTests, name, mTestsCount, id ),
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

