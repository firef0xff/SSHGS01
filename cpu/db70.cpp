#include "db70.h"
#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{

DB70::DB70():
   Och_M(mIntData[0]), //ошибка масштабирования
   Och_DM(mIntData[1]), //ошибка демасштабирования
   MotoClock(mIntData[2]), //мотопробег

   BP7(mFloatData[0]), //давление в контуре А (real)
   BP8(mFloatData[1]), //давление в контуре В (real)
   BP9(mFloatData[2]), //давление в контуре С (real)
   BP10(mFloatData[3]), //давление управления насосом (real)
   BP11(mFloatData[4]), //давление управления мотором (real)
   BP12(mFloatData[5]), //давление/разряжение всасывания насосом (real)
   BP13(mFloatData[6]), //давление в дренаже насоса (real)
   BP14(mFloatData[7]), //давление 1 (real)
   BP15(mFloatData[8]), //давление 2 (real)
   BV5(mFloatData[9]), //расход контура А до 36 л/мин (real)
   BV6(mFloatData[10]), //расход контура В до 240 л/мин (real)
   BV7(mFloatData[11]), //расход контура С до 600 л/мин (real)
   BV8(mFloatData[12]), //расход в дренаже до 3
   BV9(mFloatData[13]), //расход в дренаже до 15 л/мин (real)
   BV10(mFloatData[14]), //расход в дренаже до 90 л/мин (real)
   BT2(mFloatData[15]), //температура масла на всасывании (real)
   BT3(mFloatData[16]), //температура масла на модуле питания (real)
   BL1(mFloatData[17]), //уровень масла модуля питания (real)
   DM1(mFloatData[18]), //крутящий момент Нм (real)
   DR1(mFloatData[19]), //частота вращения об/мин (real)
   R2(mFloatData[20]), //задание регулировки давления насоса (real)
   R3(mFloatData[21]), //задание регулировки расхода насоса (real)
   reserv1(mFloatData[22]),
   reserv2(mFloatData[23]),
   reserv3(mFloatData[24]),
   YB7(mFloatData[25]), //задание давления в контуре А (real)
   YB8(mFloatData[26]), //задание давления в контуре В (real)
   YB9(mFloatData[27]), //задание давления в контуре С (real)
   YB10(mFloatData[28]), //задание давления управления насосом (real)
   YB11(mFloatData[29]), //задание на гидромотор 450-1200мА (real)
   YB12(mFloatData[30]), //задание давления на насос (real)
   YB13(mFloatData[31]), //задание расхода на насос (real)
   reserv4(mFloatData[32]),
   reserv5(mFloatData[33])
{
   memset( mIntData, 0, sizeof(mIntData) );
   memset( mFloatData, 0, sizeof(mFloatData) );
   mGroupID = opc::miniOPC::Instance().AddGroup( L"DB70", mAdresses, INT_COUNT + FLOAT_COUNT );

#ifdef DEMO
    Och_M = 1; //ошибка масштабирования
    Och_DM = 2; //ошибка демасштабирования

    BP7 = 3.1; //давление в контуре А (real)
    BP8 = 3.2; //давление в контуре В (real)
    BP9 = 3.3; //давление в контуре С (real)

    BP10 = 4.1; //давление управления насосом (real)
    BP11 = 4.2; //давление управления мотором (real)
    BP12 = 4.3; //давление/разряжение всасывания насосом (real)
    BP13 = 4.4; //давление в дренаже насоса (real)
    BP14 = 4.5; //давление 1 (real)
    BP15 = 4.6; //давление 2 (real)

    BV5 = 36; //расход контура А до 36 л/мин (real)
    BV6 = 240; //расход контура В до 240 л/мин (real)
    BV7 = 600; //расход контура С до 600 л/мин (real)

    BV8 = 3; //расход в дренаже до 3
    BV9 = 15; //расход в дренаже до 15 л/мин (real)
    BV10 = 90; //расход в дренаже до 90 л/мин (real)

    BT2 = 30; //температура масла на всасывании (real)
    BT3 = 31; //температура масла на модуле питания (real)

    BL1 = 100; //уровень масла модуля питания (real)
    DM1 = 10; //крутящий момент Нм (real)
    DR1 = 2650; //частота вращения об/мин (real)

    R2 = 5.4; //задание регулировки давления насоса (real)
    R3 = 60; //задание регулировки расхода насоса (real)

    YB7 = 6.1; //задание давления в контуре А (real)
    YB8 = 6.2; //задание давления в контуре В (real)
    YB9 = 6.3; //задание давления в контуре С (real)

    YB10 = 7; //задание давления управления насосом (real)
    YB11 = 659; //задание на гидромотор 450-1200мА (real)
    YB12 = 7.1; //задание давления на насос (real)
    YB13 = 8; //задание расхода на насос (real)
#endif
}

void DB70::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < INT_COUNT + FLOAT_COUNT; i++)
    {
        if ( i < ( INT_COUNT ) )
            mIntData[ i ] = rez[i].vDataValue.lVal;
        else
            mFloatData[ i - INT_COUNT ] = rez[i].vDataValue.fltVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
