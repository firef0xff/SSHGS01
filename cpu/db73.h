#pragma once
#include "data_block_base.h"
#include <inttypes.h>


namespace cpu
{
class CpuMemory;
namespace data
{

class DB73: public In
{
public:
   void Read();

   bool& SP16_warning; //загрязнен фильтр в контуре А
   bool& SP17_warning; //загрязнен фильтр в контуре В
   bool& SP18_warning; //загрязнен фильтр в контуре С
   bool& SP19_warning; //загрязнен фильтр линии упр.
   bool& SP20_warning; //загрязнен фильтр линии утечек
   bool& SP21_warning; //загрязнен фильтр контура охлаждения
   bool& SP22_warning; //загрязнен фильтр линии слива
   bool& SP23_warning; //нет давления в пн/системе
   bool& SP24_warning; //загрязнен фильтр дренажа
   bool& SP25_warning; //загрязнен фильтр контрол-го дренажа
   bool& SQ16_warning; //кран на подкл-ние всасывающей линии - закрыт
   bool& SQ17_warning; //кран на подкл-ние дренажной линии - закрыт
   bool& SQ18_warning; //кран на отключение всасывающей линии - закрыт
   bool& SQ19_warning; //кран закрытие дренажной линии - закрыт
   bool& SQ20_warning; //нет закрытия защитного кожуха
   bool& SQ21_warning; //нет закрытия кожуха муфты
   bool& Q_NO_Istablished; //Расход в регуляторе не установился
   bool& P_NO_Istablished; //Давление в регуляторе не установилось
   bool& OP40_Q_Nul; //Расход в нуле
   bool& OP40_P_Nul; //Давленине в нуле
   bool& OP40_Qr_NO_Dopusk; //Расход рабочий не в допуске (функц-ние)
   bool& OP41_Qr_NO_Dopusk; //Расход рабочий не в допуске (прочность)
   bool& OP42_Qr_NO_Dopusk; //Расход рабочий не в допуске (гермет-ть)
   bool& OP48_Qd_NO_Dopusk; //Расход в дренаже не в допуске

private:
   friend class cpu::CpuMemory;
   DB73();
   DB73( const DB73& ) = delete;
   void operator = ( const DB73& ) = delete;

   enum
   {
       BOOL_COUNT = 24
   };

   bool mBoolData[ BOOL_COUNT ];
   uint64_t mGroupID = 0;

   wchar_t const* mAdresses[ BOOL_COUNT ] = {
      L"CPU/DB72.SP16_warning",
      L"CPU/DB72.SP17_warning",
      L"CPU/DB72.SP18_warning",
      L"CPU/DB72.SP19_warning",
      L"CPU/DB72.SP20_warning",
      L"CPU/DB72.SP21_warning",
      L"CPU/DB72.SP22_warning",
      L"CPU/DB72.SP23_warning",
      L"CPU/DB72.SP24_warning",
      L"CPU/DB72.SP25_warning",
      L"CPU/DB72.SQ16_warning",
      L"CPU/DB72.SQ17_warning",
      L"CPU/DB72.SQ18_warning",
      L"CPU/DB72.SQ19_warning",
      L"CPU/DB72.SQ20_warning",
      L"CPU/DB72.SQ21_warning",
      L"CPU/DB72.Q_NO_Istablished",
      L"CPU/DB72.P_NO_Istablished",
      L"CPU/DB72.OP40_Q_Nul",
      L"CPU/DB72.OP40_P_Nul",
      L"CPU/DB72.OP40_Qr_NO_Dopusk",
      L"CPU/DB72.OP41_Qr_NO_Dopusk",
      L"CPU/DB72.OP42_Qr_NO_Dopusk",
      L"CPU/DB72.OP48_Qd_NO_Dopusk",
   };
};

}
}
