#include "db73.h"
#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB73::DB73():
   SP16_warning( mBoolData[0] ),//загрязнен фильтр в контуре А
   SP17_warning( mBoolData[1] ),//загрязнен фильтр в контуре В
   SP18_warning( mBoolData[2] ),//загрязнен фильтр в контуре С
   SP19_warning( mBoolData[3] ),//загрязнен фильтр линии упр.
   SP20_warning( mBoolData[4] ),//загрязнен фильтр линии утечек
   SP21_warning( mBoolData[5] ),//загрязнен фильтр контура охлаждения
   SP22_warning( mBoolData[6] ),//загрязнен фильтр линии слива
   SP23_warning( mBoolData[7] ),//нет давления в пн/системе
   SP24_warning( mBoolData[8] ),//загрязнен фильтр дренажа
   SP25_warning( mBoolData[9] ),//загрязнен фильтр контрол-го дренажа
   SQ16_warning( mBoolData[10] ),//кран на подкл-ние всасывающей линии - закрыт
   SQ17_warning( mBoolData[11] ),//кран на подкл-ние дренажной линии - закрыт
   SQ18_warning( mBoolData[12] ),//кран на отключение всасывающей линии - закрыт
   SQ19_warning( mBoolData[13] ),//кран закрытие дренажной линии - закрыт
   SQ20_warning( mBoolData[14] ),//нет закрытия защитного кожуха
   SQ21_warning( mBoolData[15] ),//нет закрытия кожуха муфты
   Q_NO_Istablished( mBoolData[16] ),//Расход в регуляторе не установился
   P_NO_Istablished( mBoolData[17] ),//Давление в регуляторе не установилось
   OP40_Q_Nul_S1( mBoolData[18] ),//Расход в нуле
   OP40_Q_Nul_S2( mBoolData[19] ),//Расход в нуле
   OP40_P_Nul_S1( mBoolData[20] ),//Давленине в нуле
   OP40_P_Nul_S2( mBoolData[21] ),//Давленине в нуле
   OP40_Qr_NO_Dopusk1( mBoolData[22] ),//Расход рабочий не в допуске (функц-ние)
   OP40_Qr_NO_Dopusk2( mBoolData[23] ),//Расход рабочий не в допуске (функц-ние)
   OP41_Qr_NO_Dopusk( mBoolData[24] ),//Расход рабочий не в допуске (прочность)
   OP42_Qr_NO_Dopusk( mBoolData[25] ),//Расход рабочий не в допуске (гермет-ть)
   OP48_Qd_NO_Dopusk( mBoolData[26] ),//Расход в дренаже не в допуске
   LevelMaslaAlarmPump( mBoolData[27] ), //аварийный уровень в баке масла 2
   TempMaslaAlarmPump1( mBoolData[28] ), //аварийная температура масла в баке 1
   TempMaslaAlarmPump2( mBoolData[29] ), //аварийная температура масла в баке 2
   LowSpeenSpeed( mBoolData[30] ), //Расчетные обороты ниже допустимых
   SpeenSpeedNotSet( mBoolData[31] ), //Обороты не установились
   Pressure1NotSet( mBoolData[32] ), //Давление в секции 1 не установилось
   Pressure2NotSet( mBoolData[33] ), //Давление в секции 2 не установилось
   ControlPressureNotSet( mBoolData[34] ),//Давление управления не установилось
   SensPressA( mBoolData[35] ),              //Обрыв/неисправность датчика давления контура А
   SensPressB( mBoolData[36] ),              //Обрыв/неисправность датчика давления контура В
   SensPressC( mBoolData[37] ),              //Обрыв/неисправность датчика давления контура С
   SensPressPumpControl( mBoolData[38] ),    //Обрыв неисправность датчика давления управления насосом
   SensPressEngineControl( mBoolData[39] ),  //Обрыв/неисправность датчика давления управления гидромотором
   SensPressIn( mBoolData[40] ),             //Обрыв/неисправность датчика давления на всасывании
   SensPressOut( mBoolData[41] ),            //Обрыв/неисправность датчика давления в дренаже насоса
   SensTempIn( mBoolData[42] ),              //Обрыв/неисправность датчика температуры масла на всасывании
   SensTempFeed( mBoolData[43] ),            //Обрыв/неисправность датчика температуры масла модуля питания
   SensLevelFeed( mBoolData[44] ),           //Обрыв/неисправность датчика уровня масла модуля питания
   SensTorque( mBoolData[45] ),              //Обрыв/неисправность датчика момента
   SensSpeenSpeed( mBoolData[46] ),           //Обрыв/неисправность датчика оборотов
   M13_Fault( mBoolData[47] ), //Нет включения М13
   M14_Fault( mBoolData[48] ), //Нет включения М14
   M15_Fault( mBoolData[49] ), //Нет включения М15
   M16_17_Fault( mBoolData[50] ), //Нет включения М16,17
   M18_19_Fault( mBoolData[51] ) //Нет включения М18,19

{
   memset( mBoolData, 0, sizeof(mBoolData) );
   mGroupID = opc::miniOPC::Instance().AddGroup( L"DB73", mAdresses, BOOL_COUNT );

#ifdef DEMO
   memset( mBoolData, 1, sizeof(mBoolData) );
#endif

}

void DB73::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < BOOL_COUNT; i++)
    {
        if ( i < ( BOOL_COUNT ) )
            mBoolData[ i ] = rez[i].vDataValue.boolVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
#ifdef DEMO
   memset( mBoolData, 1, sizeof(mBoolData) );
#endif

}

}
}
