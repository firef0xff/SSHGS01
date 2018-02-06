#include "db72.h"
#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{

DB72::DB72():
   OP40_Work( mBoolData[0] ),//Выполнение (функц-ние)
   OP40_control( mBoolData[1] ), //бит, по которому должно выплыть окошко
   OP41_Work( mBoolData[2] ),//Выполнение (прочность)
   OP42_Work( mBoolData[3] ),//Выполнение (наружная герметичность)
   OP43_Work( mBoolData[4] ),//Выполнение (КПД)
   OP44_Work( mBoolData[5] ),//Выполнение (рабочий объем)
   OP45_Work( mBoolData[6] ),//Выполнение (коэффициент подачи)
   OP46_Work( mBoolData[7] ),//Выполнение (номинальная мощность)
   OP47_Work( mBoolData[8] ),//Выполнение (подача насоса)
   OP48_Work( mBoolData[9] ),//Выполнение (расход в дренаже)
   OP49_Work( mBoolData[10] ),//Выполнение (функц-ные зависимости)
   OP40_End( mBoolData[11] ),//Конец (функц-ние)
   OP41_End( mBoolData[12] ),//Конец (прочность)
   OP42_End( mBoolData[13] ),//Конец (наружная герметичность)
   OP43_End( mBoolData[14] ),//Конец (КПД)
   OP44_End( mBoolData[15] ),//Конец (рабочий объем)
   OP45_End( mBoolData[16] ),//Конец (коэффициент подачи)
   OP46_End( mBoolData[17] ),//Конец (номинальная мощность)
   OP47_End( mBoolData[18] ),//Конец (подача насоса)
   OP48_End( mBoolData[19] ),//Конец (расход в дренаже)
   OP49_End( mBoolData[20] ),//Конец (функц-ные зависимости)
   OP49_1( mBoolData[21] ),//Выполнение - подача насоса от давления
   OP49_2( mBoolData[22] ),//Выполнение - мощность насоса от давления
   OP49_3( mBoolData[23] ),//Выполнение - коэффиц-нт подачи от давления
   OP49_4( mBoolData[24] ),//Выполнение - КПД от давления
   OP49_G1( mBoolData[25] ),//Выполнение - график 1
   OP49_G2( mBoolData[26] ),//Выполнение - график 2
   OP49_G3( mBoolData[27] ),//Выполнение - график 3
   OP49_G4( mBoolData[28] ),//Выполнение - график 4
   OP49_G5( mBoolData[29] ),//Выполнение - график 5
   OP49_G6( mBoolData[30] ),//Выполнение - график 6
   OP49_Ready( mBoolData[31] ),//данные готовы к чтению

   Q_S1( mFloatData[0] ),
   Q_S2( mFloatData[1] ),
   OP43_KPD( mFloatData[2] ),//Значение КПД
   OP44_Work_V( mFloatData[3] ),//Значение рабочего объема
   OP45_K_Podacha( mFloatData[4] ),//Значение коэффиц. подачи
   OP46_Power( mFloatData[5] ),//Значение мощности
   OP47_Podacha_1( mFloatData[6] ),//Значение подачи 1
   OP47_Podacha_2( mFloatData[7] ),//Значение подачи 2
   OP47_Podacha_3( mFloatData[8] ),//Значение подачи 3
   OP48_Q_Drenag( mFloatData[9] ) //Значение расхода в дренаже
{
   memset( mBoolData, 0, sizeof(mBoolData) );
   memset( mFloatData, 0, sizeof(mFloatData) );
   mGroupID = opc::miniOPC::Instance().AddGroup( L"DB72", mAdresses, BOOL_COUNT + FLOAT_COUNT );

}

void DB72::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < BOOL_COUNT + FLOAT_COUNT; i++)
    {
        if ( i < ( BOOL_COUNT ) )
            mBoolData[ i ] = rez[i].vDataValue.boolVal;
        else
            mFloatData[ i - BOOL_COUNT ] = rez[i].vDataValue.fltVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
