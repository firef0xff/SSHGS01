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
   bool& OP40_Q_Nul_S1; //Расход в нуле
   bool& OP40_Q_Nul_S2; //Расход в нуле
   bool& OP40_P_Nul_S1; //Давленине в нуле
   bool& OP40_P_Nul_S2; //Давленине в нуле
   bool& OP40_Qr_NO_Dopusk1; //Расход рабочий не в допуске (функц-ние)
   bool& OP40_Qr_NO_Dopusk2; //Расход рабочий не в допуске (функц-ние)
   bool& CriticalOilLvl; //Аварийный уровень масла в баке модуля питания
   bool& OP42_Qr_NO_Dopusk; //Расход рабочий не в допуске (гермет-ть)
   bool& OP48_Qd_NO_Dopusk; //Расход в дренаже не в допуске
   bool& LevelMaslaAlarmPump; //аварийный уровень в баке масла 2
   bool& TempMaslaAlarmPump1; //аварийная температура масла в баке 1
   bool& TempMaslaAlarmPump2; //аварийная температура масла в баке 2
   bool& LowSpeenSpeed;       //Расчетные обороты ниже допустимых

   bool& SpeenSpeedNotSet; //Обороты не установились
   bool& Pressure1NotSet; //Давление в секции 1 не установилось
   bool& Pressure2NotSet; //Давление в секции 2 не установилось
   bool& ControlPressureNotSet;//Давление управления не установилось

   bool& SensPressA;              //Обрыв/неисправность датчика давления контура А
   bool& SensPressB;              //Обрыв/неисправность датчика давления контура В
   bool& SensPressC;              //Обрыв/неисправность датчика давления контура С
   bool& SensPressPumpControl;    //Обрыв неисправность датчика давления управления насосом
   bool& SensPressEngineControl;  //Обрыв/неисправность датчика давления управления гидромотором
   bool& SensPressIn;             //Обрыв/неисправность датчика давления на всасывании
   bool& SensPressOut;            //Обрыв/неисправность датчика давления в дренаже насоса
   bool& SensTempIn;              //Обрыв/неисправность датчика температуры масла на всасывании
   bool& SensTempFeed;            //Обрыв/неисправность датчика температуры масла модуля питания
   bool& SensLevelFeed;           //Обрыв/неисправность датчика уровня масла модуля питания
   bool& SensTorque;              //Обрыв/неисправность датчика момента
   bool& SensSpeenSpeed;          //Обрыв/неисправность датчика оборотов

   bool& M13_Fault; //Нет включения М13
   bool& M14_Fault; //Нет включения М14
   bool& M15_Fault; //Нет включения М15
   bool& M16_17_Fault; //Нет включения М16,17
   bool& M18_19_Fault; //Нет включения М18,19

private:
   friend class cpu::CpuMemory;
   DB73();
   DB73( const DB73& ) = delete;
   void operator = ( const DB73& ) = delete;

   enum
   {
       BOOL_COUNT = 52
   };

   bool mBoolData[ BOOL_COUNT ];
   uint64_t mGroupID = 0;

   wchar_t const* mAdresses[ BOOL_COUNT ] = {
      L"CPU/DB73.SP16_warning",
      L"CPU/DB73.SP17_warning",
      L"CPU/DB73.SP18_warning",
      L"CPU/DB73.SP19_warning",
      L"CPU/DB73.SP20_warning",
      L"CPU/DB73.SP21_warning",
      L"CPU/DB73.SP22_warning",
      L"CPU/DB73.SP23_warning",
      L"CPU/DB73.SP24_warning",
      L"CPU/DB73.SP25_warning",
      L"CPU/DB73.SQ16_warning",
      L"CPU/DB73.SQ17_warning",
      L"CPU/DB73.SQ18_warning",
      L"CPU/DB73.SQ19_warning",
      L"CPU/DB73.SQ20_warning",
      L"CPU/DB73.SQ21_warning",
      L"CPU/DB73.Q_NO_Istablished",
      L"CPU/DB73.P_NO_Istablished",
      L"CPU/DB73.OP40_Q_Nul_S1",
      L"CPU/DB73.OP40_Q_Nul_S2",
      L"CPU/DB73.OP40_P_Nul_S1",
      L"CPU/DB73.OP40_P_Nul_S2",
      L"CPU/DB73.OP40_Qr_NO_Dopusk1",
      L"CPU/DB73.OP40_Qr_NO_Dopusk2",
      L"CPU/DB73.CriticalOilLvl",  //Аварийный уровень масла в баке модуля питания
      L"CPU/DB73.OP42_Qr_NO_Dopusk",
      L"CPU/DB73.OP48_Qd_NO_Dopusk",
      L"CPU/DB73.LevelMaslaAlarmPump",
      L"CPU/DB73.TempMaslaAlarmPump1",
      L"CPU/DB73.TempMaslaAlarmPump2",
      L"CPU/DB73.LowSpeenSpeed",

      L"CPU/DB73.SpeenSpeedNotSet", //Обороты не установились
      L"CPU/DB73.Pressure1NotSet", //Давление в секции 1 не установилось
      L"CPU/DB73.Pressure2NotSet", //Давление в секции 2 не установилось
      L"CPU/DB73.ControlPressureNotSet",//Давление управления не установилось

      L"CPU/DB73.SensPressA",             //Обрыв/неисправность датчика давления контура А
      L"CPU/DB73.SensPressB",             //Обрыв/неисправность датчика давления контура В
      L"CPU/DB73.SensPressC",             //Обрыв/неисправность датчика давления контура С
      L"CPU/DB73.SensPressPumpControl",   //Обрыв неисправность датчика давления управления насосом
      L"CPU/DB73.SensPressEngineControl", //Обрыв/неисправность датчика давления управления гидромотором
      L"CPU/DB73.SensPressIn",            //Обрыв/неисправность датчика давления на всасывании
      L"CPU/DB73.SensPressOut",           //Обрыв/неисправность датчика давления в дренаже насоса
      L"CPU/DB73.SensTempIn",             //Обрыв/неисправность датчика температуры масла на всасывании
      L"CPU/DB73.SensTempFeed",           //Обрыв/неисправность датчика температуры масла модуля питания
      L"CPU/DB73.SensLevelFeed",          //Обрыв/неисправность датчика уровня масла модуля питания
      L"CPU/DB73.SensTorque",             //Обрыв/неисправность датчика момента
      L"CPU/DB73.SensSpeenSpeed",         //Обрыв/неисправность датчика оборотов

      L"CPU/DB73.M13_Fault",//Нет включения М13
      L"CPU/DB73.M14_Fault",//Нет включения М14
      L"CPU/DB73.M15_Fault",//Нет включения М15
      L"CPU/DB73.M16_17_Fault",//Нет включения М16,17
      L"CPU/DB73.M18_19_Fault",//Нет включения М18,19
   };
};

}
}
