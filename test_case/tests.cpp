#include "tests.h"
#include "test_device.h"
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
                     "Испытание гидроаппаратуры управления направлением расхода" );
namespace hydro
{

namespace devices
{

using namespace examinee::hydro;
static ElectroDevice d1 ( "DS3",      "DUPLOMATIC" );
static ElectroDevice d2 ( "DL3",      "DUPLOMATIC" );
static ElectroDevice d3 ( "DL3B",     "DUPLOMATIC" );
static ElectroDevice d4 ( "MDS3",     "DUPLOMATIC" );
static ElectroDevice d5 ( "MDF3",     "DUPLOMATIC" );
static ElectroDevice d6 ( "DS5",      "DUPLOMATIC" );
static ElectroDevice d7 ( "DL5",      "DUPLOMATIC" );
static ElectroDevice d8 ( "DL5B",     "DUPLOMATIC" );
static ElectroDevice d9 ( "DD44",     "DUPLOMATIC" );
static ElectroDevice d10( "E*P4",     "DUPLOMATIC" );
static ElectroHydroDevice d11( "DSP7",     "DUPLOMATIC", 12, 210 );
static ElectroHydroDevice d12( "DSP10",    "DUPLOMATIC", 12, 280 );
static ElectroDevice d13( "DS(P)*M",  "DUPLOMATIC" );
static ElectroDevice d14( "П6",       "РОССИЯ" );
static ElectroDevice d15( "В6",       "РОССИЯ" );
static ElectroDevice d16( "1Р6",      "РОССИЯ" );
static ElectroDevice d17( "В10",      "РОССИЯ" );
static ElectroDevice d18( "1Р10",     "РОССИЯ" );
static ElectroDevice d19( "В16",      "РОССИЯ" );
static ElectroDevice d20( "1Р203",    "REXROTH" );
static ElectroDevice d21( "1Р323",    "REXROTH" );
static ElectroDevice d22( "4WE6",     "REXROTH" );
static ElectroDevice d23( "4WE10",    "REXROTH" );
static ElectroHydroDevice d24( "4WEH10…32","REXROTH", 14, 280 );
}//namespace devices

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
                     "Испытание пропорциональной и сервоаппаратуры управления направлением расхода" );
namespace servo
{
/*
namespace devices
{
using namespace examinee::servo;

static Device d1("DSE3", "DUPLOMATIC");
static Device d2("DSE3B", "DUPLOMATIC");
static Device d3("DSE3G", "DUPLOMATIC");
static Device d4("DSE3J", "DUPLOMATIC");
static Device d5("DSE5", "DUPLOMATIC");
//static Device d6("DSE5-C60/10N-D24K1");
static Device d7("DSE5G", "DUPLOMATIC");
static Device d8("DSE5J", "DUPLOMATIC");
static Device d9("DSPE*", "DUPLOMATIC");
//static Device d10("DSPE7-C150/11N-IE/D24K1");
static Device d11("DSPE*G", "DUPLOMATIC");
static Device d12("DSPE*J", "DUPLOMATIC");
static Device d13("DXJ3", "DUPLOMATIC");
static Device d14("DXJ5", "DUPLOMATIC");
static Device d15("DS(P)*M", "DUPLOMATIC");
static Device d16("PRE25", "DUPLOMATIC");
//static Device d17("PRE25-350/10N-D24K1");
static Device d18("RQE5", "DUPLOMATIC");
//static Device d19("RQE5-P6/53-24");
static Device d20("4WRAE6", "REXROTH");
static Device d21("4WRAE10", "REXROTH");
static Device d22("4WRE6", "REXROTH");
//static Device d23("4WRE 6 2X/G24K4/V");
//static Device d24("4WRE 6 E32-2X/G24K4/V");
static Device d25("4WRE10", "REXROTH");
//static Device d26("4WRE 10 E50-2X/G24K4/V");
static Device d27("4WREE6", "REXROTH");
//static Device d28("4WREE 6 E1-16-22/G24K31/F1V");
//static Device d29("4WREE 6 E16-22/G24K31/A1V");
//static Device d30("4WREE 6 E32-22/G24K31/F1V");
static Device d31("4WREE10", "REXROTH");
static Device d32("4WRSE6", "REXROTH");
//static Device d33("4WRSE 6 V10-31/G24K0/A1V");
//static Device d34("4WRSE 6V10-3X/G24K0 A1 V12V");
static Device d35("4WRSEH6", "REXROTH");
//static Device d36("4WRSEH 6 C3B24LD-30/G24K0/A1V");
//static Device d37("4WRSEH 6 C3B24LD-3X/G24K0/A1V");
static Device d38("4WRSEH10", "REXROTH");
static Device d39("4WRPEH6", "REXROTH");
//static Device d40("4WRPEH 6 C3 B12L-20/G24K0/A1M");
//static Device d41("4WRPEH 6 C3 B24L-2X/G24K0/A1M");
static Device d42("4WRPEH10", "REXROTH");
//static Device d43("4WRPEH 10 C4 B100L-2X/G24K0/A1M");
static Device d44("4WRZ10...25", "REXROTH");
//static Device d45("4WRZ 25 W8-325-70/6EG24N9EK4/M");
//static Device d46("4WRZ 25 W6-220-70/6EG24N9EK4/M");
//static Device d47("4WRZ 25 W6-220-7X/6EG24EK4/M");
//static Device d48("4WRZ 25 W8-325-7X/6EG24EK4/M");
//static Device d49("4WRZ 10 E-85-70/6EG24N9TK4/D3M");
//static Device d50("4WRZ 10 W6-85-7X/6E G24 E K4/M");
//static Device d51("4WRZ 10 E-85-7X/6EG24N9TK4/D3M");
static Device d52("4WRTE10...35", "REXROTH");
//static Device d53("4WRTE 10 E50L-41/6EG24K31/A5M");
//static Device d54("4WRTE 10 W825L-40/6EG24ETK31/A1M");
static Device d55("4WRKE16", "REXROTH");
//static Device d56("4WRKE 16 W8125L-3X/6EG24EK31F1D3M");
static Device d57("4WRZE25", "REXROTH");
//static Device d58("4WRZE 25 W6325-7X/6EG24N9K31D3M");
static Device d59("4WRPH6", "REXROTH");
//static Device d60("4WRPH 6 C3 B12L-2X/G24Z4/M");
//static Device d61("4WRPH 6 C3 B04L-20/G24Z4/M");
//static Device d62("4WRPH 6 C B40L-20/G24Z4/M-859");
//static Device d63("4WRPH 6 C B40L-2X/G24Z4/M-859");
static Device d64("4WRPH10", "REXROTH");
//static Device d65("4WRPH 10 C3 B100L-2X/G24Z4/M");
static Device d66("4WRP10", "REXROTH");
//static Device d67("4WRP 10 W63S-1X/G24Z4/M");
static Device d68("4WRLE10", "REXROTH");
//static Device d69("4WRLE10 V-85M-3X/G24 K0/A1M");
static Device d70("4WRLE16", "REXROTH");
//static Device d71("4WRLE 16 V200M-30/G24K0/A1M");
//static Device d72("4WRLE 16 V200M-3X/G24/K0/A1M");
static Device d73("4WRL16", "REXROTH");
//static Device d74("4WRL 16-W1Z180S-30/G24Z4/M");
//static Device d75("4WRL 16 W1Z180S-3X/G24Z4/M");
static Device d76("4WRL25", "REXROTH");
//static Device d77("4WRL 25 W12-350S-3X/G24Z4/M");
//static Device d78("4WRL 25 W1Z350S-3X/G24Z4/M");
//static Device d79("4WRL 25 WZ350S-3X/G24Z4/M");
static Device d80("4WSE3EE16", "REXROTH");
//static Device d81("4WSE3EE16-15/200B9T315K9EV");
static Device d82("4WSE3E16", "REXROTH");
static Device d83("SV1-10-210-01", "EMG");
//static Device d84("SV1-10/16/315/6");
//static Device d85("SV1-10/8/315/6");
//static Device d86("SV1-10/8/100/6");
//static Device d87("SV1-10/32/315/6 ");
static Device d88("KADG5V", "Vickers(ETON)");
static Device d89("D631, G631", "MOOG");
//static Device d90("D631-176C Type P20FDFMANBR");
//static Device d91("G631-3009B Type H20JDFM4VBR");
//static Device d92("D631-502С Type PХХFDGMANBR");
static Device d93("D633, D634", "MOOG");
//static Device d94("D633-419B Type R16KO1F0NSP2");
//static Device d95("D633-333B Type R16KO1F0NSS2");
static Device d96("D661", "MOOG");
//static Device d97("D661-5626C Type G18HOAO6VSB0-P");
//static Device d98("G661-623А, Type G15F0E08V6BO");
//static Device d99("D661-xxxx, Type G35JXAA6VSX2HA - 1,5:1 (90/60L)");
//static Device d100("D662-XXXX Type P02JXMF6VSX2-A");
//static Device d101("D662-Z4333K Type P01JXMF6VSX2-A -2:1");
//static Device d102("D661-6316C Type G23JXAA6VSX2HA - 2:1");
static Device d103("D682, D683", "MOOG");
static Device d104("D760, D761, G761", "MOOG");
//static Device d105("G761-3602 Type S63JOGM5UPL");
//static Device d106("G761-3602 Type S63JOGM5UP");
//static Device d107("D760F342A Type S57FOGM4VPY");
//static Device d108("F760 Type S53COFMAVPY");
//static Device d109("D760-938A, Type S063J0200NG");

}//namespace devices
*/
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



