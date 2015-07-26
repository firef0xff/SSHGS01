#include "tests.h"
#include "../devices/device.h"
#include <thread>

namespace fake
{
using namespace test;

class SucsessTest : public Test
{
public:
    SucsessTest(TestCase* tc, QString const& name, uint8_t number):
        Test(tc, name, number)
    {}

    bool Run( LogFunction to_log, bool const& stop )
    {
        to_log( "SucsessTest started" );
        for( int i=0; i<10; ++i)
        {
            to_log( "tic" );
            std::this_thread::sleep_for( std::chrono::seconds(1) );
            if (stop)
            {
                to_log( "SucsessTest interrupted" );
                return false;
            }
        }
        to_log( "BOOOOMMMMM!!!!!!" );
        return true;
    }
};

class UnSucsessTest : public Test
{
public:
    UnSucsessTest(TestCase* tc, QString const& name, uint8_t number):
        Test(tc, name, number)
    {}

    bool Run( LogFunction to_log, bool const& stop )
    {
        to_log( "UnSucsessTest started" );
        for( int i=0; i<10; ++i)
        {
            to_log( "tic" );
            std::this_thread::sleep_for( std::chrono::seconds(1) );
            if (stop)
            {
                to_log( "UnSucsessTest interrupted" );
                return false;
            }
        }
        to_log( "Phshshshshs!" );
        return false;
    }
};

}//namespace fake

namespace test
{
TestCase HydroTests( "Гидрораспределители",
                     "Испытание гидроаппаратуры управления направлением расхода" );
namespace hydro
{

namespace devices
{
using namespace examinee;
static Device d1 ( &HydroTests.Devices(), "DS3",      "DUPLOMATIC" );
static Device d2 ( &HydroTests.Devices(), "DL3",      "DUPLOMATIC" );
static Device d3 ( &HydroTests.Devices(), "DL3B",     "DUPLOMATIC" );
static Device d4 ( &HydroTests.Devices(), "MDS3",     "DUPLOMATIC" );
static Device d5 ( &HydroTests.Devices(), "MDF3",     "DUPLOMATIC" );
static Device d6 ( &HydroTests.Devices(), "DS5",      "DUPLOMATIC" );
static Device d7 ( &HydroTests.Devices(), "DL5",      "DUPLOMATIC" );
static Device d8 ( &HydroTests.Devices(), "DL5B",     "DUPLOMATIC" );
static Device d9 ( &HydroTests.Devices(), "DD44",     "DUPLOMATIC" );
static Device d10( &HydroTests.Devices(), "E*P4",     "DUPLOMATIC" );
static Device d11( &HydroTests.Devices(), "DSP7",     "DUPLOMATIC" );
static Device d12( &HydroTests.Devices(), "DSP10",    "DUPLOMATIC" );
static Device d13( &HydroTests.Devices(), "DS(P)*M",  "DUPLOMATIC" );
static Device d14( &HydroTests.Devices(), "П6",       "РОССИЯ" );
static Device d15( &HydroTests.Devices(), "В6",       "РОССИЯ" );
static Device d16( &HydroTests.Devices(), "1Р6",      "РОССИЯ" );
static Device d17( &HydroTests.Devices(), "В10",      "РОССИЯ" );
static Device d18( &HydroTests.Devices(), "1Р10",     "РОССИЯ" );
static Device d19( &HydroTests.Devices(), "В16",      "РОССИЯ" );
static Device d20( &HydroTests.Devices(), "1Р203",    "REXROTH" );
static Device d21( &HydroTests.Devices(), "1Р323",    "REXROTH" );
static Device d22( &HydroTests.Devices(), "4WE6",     "REXROTH" );
static Device d23( &HydroTests.Devices(), "4WE10",    "REXROTH" );
static Device d24( &HydroTests.Devices(), "4WEH10…32","REXROTH" );
}//namespace devices

namespace fake_case
{
using namespace fake;
static SucsessTest t1( &HydroTests, "Испытание функционирования", 1 );
static SucsessTest t2( &HydroTests, "Проверка наружной герметичности", 2 );
static UnSucsessTest t3( &HydroTests, "Проверка внутренней герметичности", 3);
static SucsessTest t4( &HydroTests, "Проверка перепада давления и зависимость перепада давления от расхода", 4);
static SucsessTest t5( &HydroTests, "Проверка максимального расхода", 5);
static UnSucsessTest t6( &HydroTests, "Проверка переключения запорно-регулирующего элемента пониженным напряжением", 6);
static SucsessTest t7( &HydroTests, "Проверка диапазона давления управления\n(для направляющей гидроаппаратуры с электрогидравлическим управлением)", 7);
static SucsessTest t8( &HydroTests, "Время срабатывания", 8 );

}//namespace fake_case

}//namespace hydro

TestCase ServoTests( "Направление расхода",
                     "Испытание пропорциональной и сервоаппаратуры управления направлением расхода" );
namespace servo
{
namespace fake2_case
{
using namespace fake;
static SucsessTest t1( &ServoTests, "Проверка аппарата пробным давлением", 1 );
static UnSucsessTest t2( &ServoTests, "Внутренняя утечка", 2 );
static SucsessTest t3( &ServoTests, "Зависимость расхода «к потребителю» от входного тока без нагрузки", 3 );
static UnSucsessTest t4( &ServoTests, "Разрешающая способность и порог вне нулевой зоны", 4 );
static SucsessTest t5( &ServoTests, "Зависимость расхода «к потребителю» от перепада давлений нагрузки", 5 );
static UnSucsessTest t6( &ServoTests, "Частотные характеристики", 6 );
static SucsessTest t7( &ServoTests, "Переходные характеристики", 7 );
}//namespace fake2_case

}//namespace servo


}



