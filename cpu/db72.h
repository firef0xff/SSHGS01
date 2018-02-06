#pragma once
#include "data_block_base.h"
#include <inttypes.h>


namespace cpu
{
class CpuMemory;
namespace data
{

class DB72: public In
{
public:
   void Read();

   bool& OP40_Work; //Выполнение (функц-ние)
   bool& OP40_control;  //это особый бит, по которому должно выплыть окошко с текстом;
                        //   «В системе отсутствуют: удары, стуки,
                        //   повышенная вибрация, резкий шум, повышенный нагрев.
                        //   Отсутствуют каплеобразования: из под крышек,
                        //   пробок, фланцев, через стыки корпусных деталей и т.д.»
   bool& OP41_Work; //Выполнение (прочность)
   bool& OP42_Work; //Выполнение (наружная герметичность)
   bool& OP43_Work; //Выполнение (КПД)
   bool& OP44_Work; //Выполнение (рабочий объем)
   bool& OP45_Work; //Выполнение (коэффициент подачи)
   bool& OP46_Work; //Выполнение (номинальная мощность)
   bool& OP47_Work; //Выполнение (подача насоса)
   bool& OP48_Work; //Выполнение (расход в дренаже)
   bool& OP49_Work; //Выполнение (функц-ные зависимости)
   bool& OP40_End; //Конец (функц-ние)
   bool& OP41_End; //Конец (прочность)
   bool& OP42_End; //Конец (наружная герметичность)
   bool& OP43_End; //Конец (КПД)
   bool& OP44_End; //Конец (рабочий объем)
   bool& OP45_End; //Конец (коэффициент подачи)
   bool& OP46_End; //Конец (номинальная мощность)
   bool& OP47_End; //Конец (подача насоса)
   bool& OP48_End; //Конец (расход в дренаже)
   bool& OP49_End; //Конец (функц-ные зависимости)
   bool& OP49_1; //Выполнение - подача насоса от давления
   bool& OP49_2; //Выполнение - мощность насоса от давления
   bool& OP49_3; //Выполнение - коэффиц-нт подачи от давления
   bool& OP49_4; //Выполнение - КПД от давления
   bool& OP49_G1; //Выполнение - график 1
   bool& OP49_G2; //Выполнение - график 2
   bool& OP49_G3; //Выполнение - график 3
   bool& OP49_G4; //Выполнение - график 4
   bool& OP49_G5; //Выполнение - график 5
   bool& OP49_G6; //Выполнение - график 6
   bool& OP49_Ready; //данные готовы к чтению

   float& Q_S1; //Расход S1 //Расход не в допуске (рабочий/дренаж)
   float& Q_S2; //Расход S2
   float& OP43_KPD; //Значение КПД
   float& OP44_Work_V; //Значение рабочего объема
   float& OP45_K_Podacha; //Значение коэффиц. подачи
   float& OP46_Power; //Значение мощности
   float& OP47_Podacha_1; //Значение подачи 1
   float& OP47_Podacha_2; //Значение подачи 2
   float& OP47_Podacha_3; //Значение подачи 3
   float& OP48_Q_Drenag; //Значение расхода в дренаже

private:
   friend class cpu::CpuMemory;
   DB72();
   DB72( const DB72& ) = delete;
   void operator = ( const DB72& ) = delete;

   enum
   {
       BOOL_COUNT = 32,
       FLOAT_COUNT = 10
   };

   bool mBoolData[ BOOL_COUNT ];
   float mFloatData[ FLOAT_COUNT ];
   uint64_t mGroupID = 0;

   wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
      L"CPU/DB72.OP40_Work",
      L"CPU/DB72.OP40_control",
      L"CPU/DB72.OP41_Work",
      L"CPU/DB72.OP42_Work",
      L"CPU/DB72.OP43_Work",
      L"CPU/DB72.OP44_Work",
      L"CPU/DB72.OP45_Work",
      L"CPU/DB72.OP46_Work",
      L"CPU/DB72.OP47_Work",
      L"CPU/DB72.OP48_Work",
      L"CPU/DB72.OP49_Work",
      L"CPU/DB72.OP40_End",
      L"CPU/DB72.OP41_End",
      L"CPU/DB72.OP42_End",
      L"CPU/DB72.OP43_End",
      L"CPU/DB72.OP44_End",
      L"CPU/DB72.OP45_End",
      L"CPU/DB72.OP46_End",
      L"CPU/DB72.OP47_End",
      L"CPU/DB72.OP48_End",
      L"CPU/DB72.OP49_End",
      L"CPU/DB72.OP49_1",
      L"CPU/DB72.OP49_2",
      L"CPU/DB72.OP49_3",
      L"CPU/DB72.OP49_4",
      L"CPU/DB72.OP49_G1",
      L"CPU/DB72.OP49_G2",
      L"CPU/DB72.OP49_G3",
      L"CPU/DB72.OP49_G4",
      L"CPU/DB72.OP49_G5",
      L"CPU/DB72.OP49_G6",
      L"CPU/DB72.OP49_Ready",

      L"CPU/DB72.Q_S1",
      L"CPU/DB72.Q_S2",
      L"CPU/DB72.OP43_KPD",
      L"CPU/DB72.OP44_Work_V",
      L"CPU/DB72.OP45_K_Podacha",
      L"CPU/DB72.OP46_Power",
      L"CPU/DB72.OP47_Podacha_1",
      L"CPU/DB72.OP47_Podacha_2",
      L"CPU/DB72.OP47_Podacha_3",
      L"CPU/DB72.OP48_Q_Drenag",
   };
};

}
}
