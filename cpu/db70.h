#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

class DB70 : public In
{
public:
   void Read();

   int& Och_M; //ошибка масштабирования
   int& Och_DM; //ошибка демасштабирования

   float& BP7; //давление в контуре А (real)
   float& BP8; //давление в контуре В (real)
   float& BP9; //давление в контуре С (real)
   float& BP10; //давление управления насосом (real)
   float& BP11; //давление управления мотором (real)
   float& BP12; //давление/разряжение всасывания насосом (real)
   float& BP13; //давление в дренаже насоса (real)
   float& BP14; //давление 1 (real)
   float& BP15; //давление 2 (real)
   float& BV5; //расход контура А до 36 л/мин (real)
   float& BV6; //расход контура В до 240 л/мин (real)
   float& BV7; //расход контура С до 600 л/мин (real)
   float& BV8; //расход в дренаже до 3
   float& BV9; //расход в дренаже до 15 л/мин (real)
   float& BV10; //расход в дренаже до 90 л/мин (real)
   float& BT2; //температура масла на всасывании (real)
   float& BT3; //температура масла на модуле питания (real)
   float& BL1; //уровень масла модуля питания (real)
   float& DM1; //крутящий момент Нм (real)
   float& DR1; //частота вращения об/мин (real)
   float& R2; //задание регулировки давления насоса (real)
   float& R3; //задание регулировки расхода насоса (real)
   float& reserv1;
   float& reserv2;
   float& reserv3;
   float& YB7; //задание давления в контуре А (real)
   float& YB8; //задание давления в контуре В (real)
   float& YB9; //задание давления в контуре С (real)
   float& YB10; //задание давления управления насосом (real)
   float& YB11; //задание на гидромотор 450-1200мА (real)
   float& YB12; //задание давления на насос (real)
   float& YB13; //задание расхода на насос (real)
   float& reserv4;
   float& reserv5;


private:
   friend class cpu::CpuMemory;
   DB70();
   DB70( const DB70& ) = delete;
   void operator = ( const DB70& ) = delete;

   enum
   {
       INT_COUNT = 2,
       FLOAT_COUNT = 34
   };

   int mIntData[ INT_COUNT ];
   float mFloatData[ FLOAT_COUNT ];
   uint64_t mGroupID = 0;

   wchar_t const* mAdresses[ INT_COUNT + FLOAT_COUNT ] = {
      L"CPU/DB70.Och_M",
      L"CPU/DB70.Och_DM",

      L"CPU/DB70.BP7",
      L"CPU/DB70.BP8",
      L"CPU/DB70.BP9",
      L"CPU/DB70.BP10",
      L"CPU/DB70.BP11",
      L"CPU/DB70.BP12",
      L"CPU/DB70.BP13",
      L"CPU/DB70.BP14",
      L"CPU/DB70.BP15",
      L"CPU/DB70.BV5",
      L"CPU/DB70.BV6",
      L"CPU/DB70.BV7",
      L"CPU/DB70.BV8",
      L"CPU/DB70.BV9",
      L"CPU/DB70.BV10",
      L"CPU/DB70.BT2",
      L"CPU/DB70.BT3",
      L"CPU/DB70.BL1",
      L"CPU/DB70.DM1",
      L"CPU/DB70.DR1",
      L"CPU/DB70.R2",
      L"CPU/DB70.R3",
      L"CPU/DB70.reserv1",
      L"CPU/DB70.reserv2",
      L"CPU/DB70.reserv3",
      L"CPU/DB70.YB7",
      L"CPU/DB70.YB8",
      L"CPU/DB70.YB9",
      L"CPU/DB70.YB10",
      L"CPU/DB70.YB11",
      L"CPU/DB70.YB12",
      L"CPU/DB70.YB13",
      L"CPU/DB70.reserv4",
      L"CPU/DB70.reserv5"
   };
};

}
}
