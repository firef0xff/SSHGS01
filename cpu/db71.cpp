#include "db71.h"

#include "miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{

DB71::DB71():
   Number_of_sections   ( mBoolData[0] ), //Количество секций в насосе (1/2)
   Direction_of_rotation( mBoolData[1] ), //Направление вращения (левое/правое)
   Electronic_control   ( mBoolData[2] ), //Наличие электр-го управления
   Hydraulic_control    ( mBoolData[3] ), //Наличие гидравл-го управления
   Type_control         ( mBoolData[4] ),     //Тип управления (дискретный/аналоговый)
   Type_current_discrete( mBoolData[5] ), //Тип тока для дискретного клапана (пост./перем.)
   Contur_A_Q1          ( mBoolData[6] ), //выбран контур А для секции 1
   Contur_B_Q1          ( mBoolData[7] ), //выбран контур В для секции 1
   Contur_C_Q1          ( mBoolData[8] ), //выбран контур С для секции 1
   Contur_A_Q2          ( mBoolData[9] ), //выбран контур А для секции 2
   Contur_B_Q2          ( mBoolData[10] ), //выбран контур В для секции 2
   Contur_C_Q2          ( mBoolData[11] ), //выбран контур С для секции 2

   Time_test_function      ( mFloatData[0] ), // Время испытаний функц-ия (0..360сек)
   Time_test_strength      ( mFloatData[1] ), //Время испытаний на прочность (1..10мин)
   Time_test_tightness     ( mFloatData[2] ), //Время испытаний наружн. герметичности (0..360сек)
   Q_N1                    ( mFloatData[3] ), //Рабочий объем насоса No1 0..355см3/об
   P_N1_min                ( mFloatData[4] ), //Миним. давление в напорной полости насоса No1
   P_N1_nominal            ( mFloatData[5] ), //Номин. давление в напорной полости насоса No1
   P_N1_max                ( mFloatData[6] ), //Максим. давление в напорной полости насоса No1 (0..45
   Q_N2                    ( mFloatData[7] ), //Рабочий объем насоса No2 (0..355см3/об)
   P_N2_min                ( mFloatData[8] ), //Миним. давление в напорной полости насоса No2 (0..450
   P_N2_nominal            ( mFloatData[9] ), //Номин. давление в напорной полости насоса No2 (0..450
   P_N2_max                ( mFloatData[10] ), //Максим. давление в напорной полости насоса No2 (0..45
   Rotat_speed_pump_min    ( mFloatData[11] ), //Миним. частота вращения насоса(ов) (200..4000об/мин)
   Rotat_speed_pump_nominal( mFloatData[12] ), //Номин. частота вращения насоса(ов) (200..4000об/мин)
   Rotat_speed_pump_max    ( mFloatData[13] ), //Максим. частота вращения насоса(ов) (200..4000об/мин
   KPD_pump                ( mFloatData[14] ), //Объемный КПД насоса
   Drainage_pump           ( mFloatData[15] ), //Дренаж насоса при номин.Р и Q (0,7..90л/мин)
   Voltage                 ( mFloatData[16] ), //Напряжение питания, В (24/48/110/220)
   Max_P_control           ( mFloatData[17] ), //Макс. давление в канале управления (0..100бар)
   Time_set_P              ( mFloatData[18] ), //Время набора давления - Х (1..60сек)
   Time_set_rotation       ( mFloatData[19] ), //Время набора частоты вращения - Y (1..60сек)
   Time_stabil_flow        ( mFloatData[20] ), //Время стабилизации потока - Z (1..600сек)
   Coeffic_function        ( mFloatData[21] ), //Коэффициент функц-ния - А (0..1%)
   Coeffic_function_drainag( mFloatData[22] ), //Коэффициент функц-ния для дренажа - А1 (0..1%)
   Precision_test_function ( mFloatData[23] ), //Точность для испыт. функц-ные зависимости - E (0,5..
   Time_function_dependence( mFloatData[24] ), //Время испыт. функц-ные зависимости - В (1..360)
   Temperature_Work        ( mFloatData[25] ), //Рабочая температура масла в баке
   SignalLevel             ( mFloatData[26] )  //Уровень сигнала управления
{
   memset( mBoolData, 0, sizeof(mBoolData) );
   memset( mFloatData, 0, sizeof(mFloatData) );
   mGroupID = opc::miniOPC::Instance().AddGroup( L"DB71", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB71::Write()
{
   HRESULT res = E_FAIL;
   while ( res == E_FAIL )
       res = opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );

   res = E_FAIL;
   while ( res == E_FAIL )
       res = opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );

}

}
}
