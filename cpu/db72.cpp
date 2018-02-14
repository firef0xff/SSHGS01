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
   OP48_Q_Drenag( mFloatData[9] ), //Значение расхода в дренаже
   Exp1( mFloatData[10] ), //расход  в секции 1
   Exp2( mFloatData[11] ), //расход в секции 2
   Press1( mFloatData[12] ), //давления  в секции 1
   Press2( mFloatData[13] ), //давление в секции 2
   Frequency( mFloatData[14] ), //частота вращения
   Torque( mFloatData[15] ), //крутящий момент
   MaxExp1( mFloatData[16] ), //максимальный расход секция 1
   MinExp1( mFloatData[17] ), //миниимальный расход секция 1
   MaxExp2( mFloatData[18] ), //максимальный расход секция 2
   MinExp2( mFloatData[19] ), //миниимальный расход секция 2
   FrequencyMin( mFloatData[20] ), //минимальная частота вращения;
   FrequencyMax( mFloatData[21] ), //номимальная частота вращения;
   FrequencyNom( mFloatData[22] ), //максимальная частота вращения;
   OP49_SpeenSpeed1( mFloatData[23] ),  //частота вращения при первом замере
   OP49_SpeenSpeed2( mFloatData[24] ),  //частота вращения при втором замере

   OP49_Count( mIntData[0] )

{
   memset( mBoolData, 0, sizeof(mBoolData) );
   memset( mFloatData, 0, sizeof(mFloatData) );
   memset( mIntData, 0, sizeof(mIntData) );
   mGroupID = opc::miniOPC::Instance().AddGroup( L"DB72", mAdresses, BOOL_COUNT + FLOAT_COUNT + INT_COUNT);

}

void DB72::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < BOOL_COUNT + FLOAT_COUNT + INT_COUNT; i++)
    {
        if ( i < ( BOOL_COUNT ) )
            mBoolData[ i ] = rez[i].vDataValue.boolVal;
        else if ( i < ( BOOL_COUNT + FLOAT_COUNT ) )
            mFloatData[ i - BOOL_COUNT ] = rez[i].vDataValue.fltVal;
        else
            mIntData[ i - (BOOL_COUNT + FLOAT_COUNT) ] = rez[i].vDataValue.lVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
