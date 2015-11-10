#include "test_base.h"
#include "../tests.h"
#include "../../settings/settings.h"
#include <thread>
namespace test
{

namespace hydro
{

uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::Test( &HydroTests, name, mTestsCount, id ),
    OilTemp(0)
{
    ++mTestsCount;
}

void Test::Start()
{
    mCommand.N_Operation = mId;
    mCommand.Start_Oper = true;
    mCommand.Stop_Oper = false;
    mCommand.Nasos_M2 = app::Settings::Instance().MainPupm() == "M2";
    mCommand.Write();
    StartTime.start();
}
void Test::Wait( bool& work, bool& done)
{
    work = false;
    done = false;
    while( !done )
    {
        std::this_thread::sleep_for( std::chrono::seconds(1) );
        mResults.Read();
    }
    TestingTime = StartTime.elapsed()/1000;
}

QJsonObject Test::Serialise() const
{
    QJsonObject obj;
    obj.insert("OilTemp", OilTemp );
    obj.insert("TestingTime", TestingTime );

    return obj;
}
bool Test::Deserialize( QJsonObject const& obj )
{
    OilTemp = obj.value("OilTemp").toDouble();
    TestingTime = obj.value("TestingTime").toInt();
    return true;
}
}//namespace hydro

namespace servo
{

uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::Test( &ServoTests, name, mTestsCount, id )
{
    ++mTestsCount;
}
}//namespace servo

namespace control_board
{
uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::Test( &ControlBoardTests, name, mTestsCount, id )
{
    ++mTestsCount;
}
}//namespace control_board

namespace hydro_cylinder
{
uint8_t Test::mTestsCount = 1;

Test::Test( QString const& name, uint8_t id ):
    test::Test( &HydroCylinder, name, mTestsCount, id )
{
    ++mTestsCount;
}
}//namespace hydro_cylinder



}//namespace test

