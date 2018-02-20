#include "db82.h"
#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{

DB82::DB82():
   Start_Stop ( mBoolData[0]), //Пуск/стоп основные насосы
   PumpManual ( mBoolData[1]),   //m155.0 - включение ручного режима
   YA27 ( mBoolData[2]),         //m155.1 - включение клапана YA27


   Q1_man ( mFloatData[0] ), //задание рабочего объема 1 насоса (см3)
   Q2_man ( mFloatData[1] ), //задание рабочего объема 2 насоса (см3)
   Speed_man ( mFloatData[2] ), //задание скорости вращения (об/мин)
   YB7_man ( mFloatData[3] ), //задание давления на клапан YB7 (бар)
   YB8_man ( mFloatData[4] ), //задание давления на клапан YB8 (бар)
   YB9_man ( mFloatData[5] ), //задание давления на клапан YB9 (бар)
   YB10_man ( mFloatData[6] ), //задание давления на клапан YB10 (бар)
   Voltage_man ( mFloatData[7] ), //напряжение питания дискретных катушек
   Type_Voltage_man ( mFloatData[8] ) //Тип напряжения (постоянное/переменное)
{
   memset( mBoolData, 0, sizeof(mBoolData) );
   memset( mFloatData, 0, sizeof(mFloatData) );
   mGroupID = opc::miniOPC::Instance().AddGroup( L"DB82", mAdresses, BOOL_COUNT + FLOAT_COUNT );

}

void DB82::Reset()
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    Write();
}
void DB82::Write()
{
    HRESULT res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );
    res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
}

DB82R::DB82R():
   Start_Stop ( mBoolData[0]), //Пуск/стоп основные насосы
   PumpManual ( mBoolData[1]),   //m155.0 - включение ручного режима
   YA27 ( mBoolData[2]),         //m155.1 - включение клапана YA27


   Q1_man ( mFloatData[0] ), //задание рабочего объема 1 насоса (см3)
   Q2_man ( mFloatData[1] ), //задание рабочего объема 2 насоса (см3)
   Speed_man ( mFloatData[2] ), //задание скорости вращения (об/мин)
   YB7_man ( mFloatData[3] ), //задание давления на клапан YB7 (бар)
   YB8_man ( mFloatData[4] ), //задание давления на клапан YB8 (бар)
   YB9_man ( mFloatData[5] ), //задание давления на клапан YB9 (бар)
   YB10_man ( mFloatData[6] ), //задание давления на клапан YB10 (бар)
   Voltage_man ( mFloatData[7] ), //напряжение питания дискретных катушек
   Type_Voltage_man ( mFloatData[8] ) //Тип напряжения (постоянное/переменное)
{
   memset( mBoolData, 0, sizeof(mBoolData) );
   memset( mFloatData, 0, sizeof(mFloatData) );
   mGroupID = opc::miniOPC::Instance().AddGroup( L"DB82R", mAdresses, BOOL_COUNT + FLOAT_COUNT );

}


void DB82R::Read()
{
   OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
   if (!rez)
   {
       //ошибка подключения..
       return;
   }
   for (size_t i = 0; i < BOOL_COUNT + FLOAT_COUNT; i++)
   {
       if ( i < BOOL_COUNT )
           mBoolData[ i ] = rez[i].vDataValue.boolVal;
       else
          mFloatData[ i - BOOL_COUNT ] = rez[i].vDataValue.fltVal;
   }
   opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );}

}
}
