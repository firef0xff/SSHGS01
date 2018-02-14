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
   OP40_Q_Nul( mBoolData[18] ),//Расход в нуле
   OP40_P_Nul( mBoolData[19] ),//Давленине в нуле
   OP40_Qr_NO_Dopusk1( mBoolData[20] ),//Расход рабочий не в допуске (функц-ние)
   OP40_Qr_NO_Dopusk2( mBoolData[21] ),//Расход рабочий не в допуске (функц-ние)
   OP41_Qr_NO_Dopusk( mBoolData[22] ),//Расход рабочий не в допуске (прочность)
   OP42_Qr_NO_Dopusk( mBoolData[23] ),//Расход рабочий не в допуске (гермет-ть)
   OP48_Qd_NO_Dopusk( mBoolData[24] ),//Расход в дренаже не в допуске
   LevelMaslaAlarmPump( mBoolData[25] ), //аварийный уровень в баке масла 2
   TempMaslaAlarmPump1( mBoolData[26] ), //аварийная температура масла в баке 1
   TempMaslaAlarmPump2( mBoolData[27] ), //аварийная температура масла в баке 2
   LowSpeenSpeed( mBoolData[28] ) //Расчетные обороты ниже допустимых

{
   memset( mBoolData, 0, sizeof(mBoolData) );
   mGroupID = opc::miniOPC::Instance().AddGroup( L"DB73", mAdresses, BOOL_COUNT );

#ifdef DEMO
   SP16_warning = true;
   SP17_warning = true;
   SP18_warning = true;
   SP19_warning = true;
   SP20_warning = true;
   SP21_warning = true;
   SP22_warning = true;
   SP23_warning = true;
   SP24_warning = true;
   SP25_warning = true;
   SQ16_warning = true;
   SQ17_warning = true;
   SQ18_warning = true;
   SQ19_warning = true;
   SQ20_warning = true;
   SQ21_warning = true;
   Q_NO_Istablished = true;
   P_NO_Istablished = true;
   OP40_Q_Nul = true;
   OP40_P_Nul = true;
   OP40_Qr_NO_Dopusk1 = true;
   OP40_Qr_NO_Dopusk2 = true;
   OP41_Qr_NO_Dopusk = true;
   OP42_Qr_NO_Dopusk = true;
   OP48_Qd_NO_Dopusk = true;
   LevelMaslaAlarmPump = true;
   TempMaslaAlarmPump1 = true;
   TempMaslaAlarmPump2 = true;
   LowSpeenSpeed = true;
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
   SP16_warning = true;
   SP17_warning = true;
   SP18_warning = true;
   SP19_warning = true;
   SP20_warning = true;
   SP21_warning = true;
   SP22_warning = true;
   SP23_warning = true;
   SP24_warning = true;
   SP25_warning = true;
   SQ16_warning = true;
   SQ17_warning = true;
   SQ18_warning = true;
   SQ19_warning = true;
   SQ20_warning = true;
   SQ21_warning = true;
   Q_NO_Istablished = true;
   P_NO_Istablished = true;
   OP40_Q_Nul = true;
   OP40_P_Nul = true;
   OP40_Qr_NO_Dopusk1 = true;
   OP40_Qr_NO_Dopusk2 = true;
   OP41_Qr_NO_Dopusk = true;
   OP42_Qr_NO_Dopusk = true;
   OP48_Qd_NO_Dopusk = true;
   LevelMaslaAlarmPump = true;
   TempMaslaAlarmPump1 = true;
   TempMaslaAlarmPump2 = true;
   LowSpeenSpeed = true;
#endif
}


}
}
