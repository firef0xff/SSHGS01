#include "tests.h"
#include <thread>
#include "implementation/hydro/functional_test.h"

namespace fake
{
using namespace test;

class SucsessTest : public Test
{
public:
    SucsessTest(TestCase* tc, QString const& name, uint8_t number, uint8_t id):
        Test(tc, name, number, id)
    {}

    bool Run()
    {
        Log( "SucsessTest started" );
        for( int i=0; i<10; ++i)
        {
            Log( "tic" );
            std::this_thread::sleep_for( std::chrono::seconds(1) );
            if (IsStopped())
            {
                Log( "SucsessTest interrupted" );
                std::this_thread::sleep_for( std::chrono::seconds(10) );
                return false;
            }
        }
        Log( "BOOOOMMMMM!!!!!!" );
        return true;
    }
};

class UnSucsessTest : public Test
{
public:
    UnSucsessTest(TestCase* tc, QString const& name, uint8_t number, uint8_t id):
        Test(tc, name, number, id)
    {}

    bool Run()
    {
        Log( "UnSucsessTest started" );
        for( int i=0; i<10; ++i)
        {
            Log( "tic" );
            std::this_thread::sleep_for( std::chrono::seconds(1) );
            if ( IsStopped() )
            {
                Log( "UnSucsessTest interrupted" );
                std::this_thread::sleep_for( std::chrono::seconds(10) );
                return false;
            }
        }
        Log( "Phshshshshs!" );
        return false;
    }
};

}//namespace fake

namespace test
{
TestCase HydroTests( "Гидрораспределители",
                     "devices/hydro.json",
                     "Испытание гидроаппаратуры управления направлением расхода");
namespace hydro
{
namespace test_case
{
static FunctionalTest t1;
}
namespace fake_case
{
using namespace fake;
static SucsessTest t2( &HydroTests, "Проверка наружной герметичности", 2, 1 );
static UnSucsessTest t3( &HydroTests, "Проверка внутренней герметичности", 3, 2);
static SucsessTest t4( &HydroTests, "Проверка перепада давления и зависимость перепада давления от расхода", 4, 3);
static SucsessTest t5( &HydroTests, "Проверка максимального расхода", 5, 4);
static UnSucsessTest t6( &HydroTests, "Проверка переключения запорно-регулирующего элемента пониженным напряжением", 6, 5);
static SucsessTest t7( &HydroTests, "Проверка диапазона давления управления\n(для направляющей гидроаппаратуры с электрогидравлическим управлением)", 7, 6);
static SucsessTest t8( &HydroTests, "Время срабатывания", 8, 7 );

}//namespace fake_case

}//namespace hydro

TestCase ServoTests( "Направление расхода",
                     "devices/servo.json",
                     "Испытание пропорциональной и сервоаппаратуры управления направлением расхода"                      );
namespace servo
{
namespace fake_case
{
using namespace fake;
static SucsessTest t1( &ServoTests, "Проверка аппарата пробным давлением", 1, 8 );
static UnSucsessTest t2( &ServoTests, "Внутренняя утечка", 2, 9 );
static SucsessTest t3( &ServoTests, "Зависимость расхода «к потребителю» от входного тока без нагрузки", 3, 10 );
static UnSucsessTest t4( &ServoTests, "Разрешающая способность и порог вне нулевой зоны", 4, 11 );
static SucsessTest t5( &ServoTests, "Зависимость расхода «к потребителю» от перепада давлений нагрузки", 5, 12 );
static UnSucsessTest t6( &ServoTests, "Частотные характеристики", 6, 13 );
static SucsessTest t7( &ServoTests, "Переходные характеристики", 7, 14 );
}//namespace fake2_case

}//namespace servo


}



