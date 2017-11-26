#include "tests.h"
#include <thread>
#include "implementation/hydro/functional_test.h"
#include "implementation/hydro/herm_test.h"
#include "implementation/hydro/pressure_duration_from_expediture.h"
#include "implementation/hydro/max_expenditure.h"
#include "implementation/hydro/switchtest.h"
#include "implementation/hydro/pressure_range.h"
#include "implementation/hydro/activation_time.h"

#include "implementation/servo/herm_tests.h"
#include "implementation/servo/expenditure_from_input.h"
#include "implementation/servo/expenditure_from_pressure_duration.h"
#include "implementation/servo/frequency_characteristics.h"
#include "implementation/servo/transient_performance.h"

#include "implementation/control_board/power_test.h"
#include "implementation/control_board/va_characteristic.h"

#include "implementation/hydro_cylinder/cylinder_test.h"

#include "implementation/pump/pump_test_1.h"
#include "implementation/pump/pump_test_2.h"
#include "implementation/pump/pump_test_3.h"
#include "implementation/pump/pump_test_4.h"

namespace fake
{
using namespace test;

class SucsessTest : public Test
{
public:
    SucsessTest(TestCase* tc, QString const& name, uint8_t number, uint8_t id):
        Test(tc, name, number, id)
    {}

    virtual QJsonObject Serialise(){ return QJsonObject(); }
    virtual bool Deserialize( QJsonObject const& ){ return true; }

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

    virtual QJsonObject Serialise(){ return QJsonObject(); }
    virtual bool Deserialize( QJsonObject const& ){ return true; }

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
static OutsideHermTest t2;
static InsideHermTest t3;
static MaxExpenditureTest t4;
static PressureDurationFromExpenditure t5;
//static SwitchTest t6;
static PressureRange t7;
//static ActivationTime t8;
}

}//namespace hydro

TestCase ServoTests( "Направление расхода",
                     "devices/servo.json",
                     "Испытание пропорциональной и сервоаппаратуры управления направлением расхода"                      );
namespace servo
{
namespace test_case
{
static OutsideHermTest t1;
static InsideHermTest t2;
static ExpeditureFromInput t3;
static ExpeditureFromPressureDuration t4;
static FrequencyCharacteristics t5;
static TransientPerformance t6;
}//namespace test_case


}//namespace servo

TestCase ControlBoardTests( "Платы управления",
                                   "devices/control_board.json",
                                   "Испытание плат управления пропорциональной аппарутуры");
namespace control_board
{
namespace test_case
{

static PowerTest t1;
static VACharacteristic t2;

}//namespace test_case
using namespace fake;

}//namespace control_board

TestCase HydroCylinder( "Гидроцилиндры",
                               "",
                               "Испытание гидроцилиндров");

namespace hydro_cylinder
{
namespace test_case
{
static FunctionalTest t1;
}//namespace test_case
}//namespace hydro_cylinder


TestCase Pumps( "Гидронасосы",
                "",
                "Испытание гидронасосов" );

namespace pump
{
namespace test_case
{
static PumpTest1 t1;
static PumpTest2 t2;
static PumpTest3 t3;
static PumpTest4 t4;
}//namespace test_case
}//namespace pump

}



