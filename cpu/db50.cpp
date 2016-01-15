#include "db50.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{

DB50::DB50():
    Osh_M(mIntData[0]),
    Osh_DM(mIntData[1]),

    BP1(mFloatData[0]),
    BP2(mFloatData[1]),
    BP3(mFloatData[2]),
    BP4(mFloatData[3]),
    BP5(mFloatData[4]),
    BP6(mFloatData[5]),
    BT1(mFloatData[6]),
    BV1(mFloatData[7]),
    BV2(mFloatData[8]),
    BV4(mFloatData[9]),
    BV3(mFloatData[10]),
    V1_do60V(mFloatData[11]),
    V0_300B(mFloatData[12]),
    A1(mFloatData[13]),
    A2(mFloatData[14]),
    A_Plata(mFloatData[15]),
    YB1(mFloatData[16]),        /*w*/
    YB2(mFloatData[17]),        /*w*/
    YB3(mFloatData[18]),        /*w*/
    YB4(mFloatData[19]),        /*w*/
    YB5(mFloatData[20]),        /*w*/
    YB6(mFloatData[21]),        /*w*/
    Z_U_Post(mFloatData[22]),   /*w*/
    Z_U_Per(mFloatData[23]),    /*w*/
    Plata_1(mFloatData[24]),
    Plata_2(mFloatData[25]),
    Plata_3(mFloatData[26]),
    Plata_4_5_6(mFloatData[27]),
    Plata_7(mFloatData[28]),
    Plata_8(mFloatData[29]),
    Plata_860mA(mFloatData[30]),
    POS_1_REAL(mFloatData[31]),
    POS_2_REAL(mFloatData[32]),
    POS_3_REAL(mFloatData[33])
{
    memset( mIntData, 0, sizeof(mIntData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB50", mAdresses, INT_COUNT + FLOAT_COUNT );
}

void DB50::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < INT_COUNT + FLOAT_COUNT; i++)
    {
        if ( i < INT_COUNT )
            mIntData[ i ] = rez[i].vDataValue.lVal;
        else
            mFloatData[ i - INT_COUNT ] = rez[i].vDataValue.fltVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

void DB50::WriteTask()
{
    HRESULT res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, INT_COUNT + 16, 8, static_cast<void*>( &YB1 ), opc::tFLOAT );
}

}
}
