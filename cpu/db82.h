#pragma once
#include "data_block_base.h"
#include <inttypes.h>


namespace cpu
{
class CpuMemory;
namespace data
{

class DB82: public Out
{
public:
   void Write();
   void Reset();

   bool& Start_Stop; //Пуск/стоп основные насосы
   bool& PumpManual;   //m155.0 - включение ручного режима
   bool& YA27;         //m155.1 - включение клапана YA27

   float& Q1_man; //задание рабочего объема 1 насоса (см3)
   float& Q2_man; //задание рабочего объема 2 насоса (см3)
   float& Speed_man; //задание скорости вращения (об/мин)
   float& YB7_man; //задание давления на клапан YB7 (бар)
   float& YB8_man; //задание давления на клапан YB8 (бар)
   float& YB9_man; //задание давления на клапан YB9 (бар)
   float& YB10_man; //задание давления на клапан YB10 (бар)
   float& Voltage_man; //напряжение питания дискретных катушек
   float& Type_Voltage_man; //Тип напряжения (постоянное/переменное)



private:
   friend class cpu::CpuMemory;
   DB82();
   DB82( const DB82& ) = delete;
   void operator = ( const DB82& ) = delete;

   enum
   {
       BOOL_COUNT = 3,
       FLOAT_COUNT = 9
   };

   bool mBoolData[ BOOL_COUNT ];
   float mFloatData[ FLOAT_COUNT ];
   uint64_t mGroupID = 0;

   wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
      L"CPU/DB82.Start_Stop",
      L"CPU/DB82.PumpManual",
      L"CPU/DB82.YA27",

      L"CPU/DB82.Q1_man",
      L"CPU/DB82.Q2_man",
      L"CPU/DB82.Speed_man",
      L"CPU/DB82.YB7_man",
      L"CPU/DB82.YB8_man",
      L"CPU/DB82.YB9_man",
      L"CPU/DB82.YB10_man",
      L"CPU/DB82.Voltage_man",
      L"CPU/DB82.Type_Voltage_man",
   };
};

class DB82R: public In
{
public:
   void Read();

   bool& Start_Stop; //Пуск/стоп основные насосы
   bool& PumpManual;   //m155.0 - включение ручного режима
   bool& YA27;         //m155.1 - включение клапана YA27

   float& Q1_man; //задание рабочего объема 1 насоса (см3)
   float& Q2_man; //задание рабочего объема 2 насоса (см3)
   float& Speed_man; //задание скорости вращения (об/мин)
   float& YB7_man; //задание давления на клапан YB7 (бар)
   float& YB8_man; //задание давления на клапан YB8 (бар)
   float& YB9_man; //задание давления на клапан YB9 (бар)
   float& YB10_man; //задание давления на клапан YB10 (бар)
   float& Voltage_man; //напряжение питания дискретных катушек
   float& Type_Voltage_man; //Тип напряжения (постоянное/переменное)



private:
   friend class cpu::CpuMemory;
   DB82R();
   DB82R( const DB82R& ) = delete;
   void operator = ( const DB82& ) = delete;

   enum
   {
       BOOL_COUNT = 3,
       FLOAT_COUNT = 9
   };

   bool mBoolData[ BOOL_COUNT ];
   float mFloatData[ FLOAT_COUNT ];
   uint64_t mGroupID = 0;

   wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
      L"CPU/DB82.Start_Stop",
      L"CPU/DB82.PumpManual",
      L"CPU/DB82.YA27",

      L"CPU/DB82.Q1_man",
      L"CPU/DB82.Q2_man",
      L"CPU/DB82.Speed_man",
      L"CPU/DB82.YB7_man",
      L"CPU/DB82.YB8_man",
      L"CPU/DB82.YB9_man",
      L"CPU/DB82.YB10_man",
      L"CPU/DB82.Voltage_man",
      L"CPU/DB82.Type_Voltage_man",
   };
};

}
}
