#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{
class DB71 :public Out
{
public:
   void Write();

   bool& Number_of_sections; //Количество секций в насосе (1/2)
   bool& Direction_of_rotation; //Направление вращения (левое/правое)
   bool& Electronic_control; //Наличие электр-го управления
   bool& Hydraulic_control; //Наличие гидравл-го управления
   bool& Type_control;     //Тип управления (дискретный/аналоговый)
   bool& Type_current_discrete; //Тип тока для дискретного клапана (пост./перем.)

   float& Time_test_function; // Время испытаний функц-ия (0..360сек)
   float& Time_test_strength; //Время испытаний на прочность (1..10мин)
   float& Time_test_tightness; //Время испытаний наружн. герметичности (0..360сек)
   float& Q_N1; //Рабочий объем насоса No1 0..355см3/об
   float& P_N1_min; //Миним. давление в напорной полости насоса No1
   float& P_N1_nominal; //Номин. давление в напорной полости насоса No1
   float& P_N1_max; //Максим. давление в напорной полости насоса No1 (0..45
   float& Q_N2; //Рабочий объем насоса No2 (0..355см3/об)
   float& P_N2_min; //Миним. давление в напорной полости насоса No2 (0..450
   float& P_N2_nominal; //Номин. давление в напорной полости насоса No2 (0..450
   float& P_N2_max; //Максим. давление в напорной полости насоса No2 (0..45
   float& Rotat_speed_pump_min; //Миним. частота вращения насоса(ов) (200..4000об/мин)
   float& Rotat_speed_pump_nominal; //Номин. частота вращения насоса(ов) (200..4000об/мин)
   float& Rotat_speed_pump_max; //Максим. частота вращения насоса(ов) (200..4000об/мин
   float& KPD_pump; //Объемный КПД насоса
   float& Drainage_pump; //Дренаж насоса при номин.Р и Q (0,7..90л/мин)
   float& Voltage; //Напряжение питания, В (24/48/110/220)
   float& Max_P_control; //Макс. давление в канале управления (0..100бар)
   float& Time_set_P; //Время набора давления - Х (1..60сек)
   float& Time_set_rotation; //Время набора частоты вращения - Y (1..60сек)
   float& Time_stabil_flow; //Время стабилизации потока - Z (1..600сек)
   float& Coeffic_function; //Коэффициент функц-ния - А (0..1%)
   float& Coeffic_function_drainag; //Коэффициент функц-ния для дренажа - А1 (0..1%)
   float& Precision_test_function; //Точность для испыт. функц-ные зависимости - E (0,5..
   float& Time_function_dependence; //Время испыт. функц-ные зависимости - В (1..360)
   float& Temperature_Work; //Рабочая температура масла в баке

private:
   friend class cpu::CpuMemory;
   DB71();
   DB71( const DB71& ) = delete;
   void operator = ( const DB71& ) = delete;

   enum
   {
       BOOL_COUNT = 6,
       FLOAT_COUNT = 26
   };

   bool mBoolData[ BOOL_COUNT ];
   float mFloatData[ FLOAT_COUNT ];
   uint64_t mGroupID = 0;

   wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
      L"CPU/DB71.Number_of_sections",
      L"CPU/DB71.Direction_of_rotation",
      L"CPU/DB71.Electronic_control",
      L"CPU/DB71.Hydraulic_control",
      L"CPU/DB71.Type_control",
      L"CPU/DB71.Type_current_discrete",

      L"CPU/DB71.Time_test_function",
      L"CPU/DB71.Time_test_strength",
      L"CPU/DB71.Time_test_tightness",
      L"CPU/DB71.Q_N1",
      L"CPU/DB71.P_N1_min",
      L"CPU/DB71.P_N1_nominal",
      L"CPU/DB71.P_N1_max",
      L"CPU/DB71.Q_N2",
      L"CPU/DB71.P_N2_min",
      L"CPU/DB71.P_N2_nominal",
      L"CPU/DB71.P_N2_max",
      L"CPU/DB71.Rotat_speed_pump_min",
      L"CPU/DB71.Rotat_speed_pump_nominal",
      L"CPU/DB71.Rotat_speed_pump_max",
      L"CPU/DB71.KPD_pump",
      L"CPU/DB71.Drainage_pump",
      L"CPU/DB71.Voltage",
      L"CPU/DB71.Max_P_control",
      L"CPU/DB71.Time_set_P",
      L"CPU/DB71.Time_set_rotation",
      L"CPU/DB71.Time_stabil_flow",
      L"CPU/DB71.Coeffic_function",
      L"CPU/DB71.Coeffic_function_drainag",
      L"CPU/DB71.Precision_test_function",
      L"CPU/DB71.Time_function_dependence",
      L"CPU/DB71.Temperature_Work"
   };
};
}
}
