#include "tests.h"

namespace fake
{
using namespace test;

class SucsessTest : public Test
{
public:
    SucsessTest(TestCase* tc, QString const& name, uint8_t number):
        Test(tc, name, number)
    {}

    bool Run()
    {
        return true;
    }
};

class UnSucsessTest : public Test
{
public:
    UnSucsessTest(TestCase* tc, QString const& name, uint8_t number):
        Test(tc, name, number)
    {}

    bool Run()
    {
        return false;
    }
};

}//namespace fake

namespace test
{
TestCase HydroTests( "Гидрораспределители",
                     "Испытание гидроаппаратуры управления направлением расхода" );

namespace fake1
{
using namespace fake;
static SucsessTest t1( &HydroTests, "Испытание функционирования", 1 );
static SucsessTest t2( &HydroTests, "Проверка наружной герметичности", 2 );
static SucsessTest t3( &HydroTests, "Проверка внутренней герметичности", 3);
static SucsessTest t4( &HydroTests, "Проверка перепада давления и зависимость перепада давления от расхода", 4);
static SucsessTest t5( &HydroTests, "Проверка максимального расхода", 5);
static SucsessTest t6( &HydroTests, "Проверка переключения запорно-регулирующего элемента пониженным напряжением", 6);
static SucsessTest t7( &HydroTests, "Проверка диапазона давления управления (для направляющей гидроаппаратуры с электрогидравлическим управлением)", 7);
static SucsessTest t8( &HydroTests, "Время срабатывания", 8 );

}//namespace

TestCase ServoTests( "Направление расхода",
                     "Испытание пропорциональной и сервоаппаратуры управления направлением расхода" );
namespace fake2
{
using namespace fake;
static SucsessTest t1( &ServoTests, "Проверка аппарата пробным давлением", 1 );
static SucsessTest t2( &ServoTests, "Внутренняя утечка", 2 );
static SucsessTest t3( &ServoTests, "Зависимость расхода «к потребителю» от входного тока без нагрузки", 3 );
static SucsessTest t4( &ServoTests, "Разрешающая способность и порог вне нулевой зоны", 4 );
static SucsessTest t5( &ServoTests, "Зависимость расхода «к потребителю» от перепада давлений нагрузки", 5 );
static SucsessTest t6( &ServoTests, "Частотные характеристики", 6 );
static SucsessTest t7( &ServoTests, "Переходные характеристики", 7 );

}//namespace

}



