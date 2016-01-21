#include "db50.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{

DB50::DB50():
    ReadyToWork( mBoolData[0] ),
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
    A_Plata(mFloatData[15]),//---
    YB1(mFloatData[30]),        /*w*/
    YB2(mFloatData[31]),        /*w*/
    YB3(mFloatData[16]),
    YB4(mFloatData[17]),
    YB5(mFloatData[32]),        /*w*/
    YB6(mFloatData[33]),        /*w*/
    Z_U_Post(mFloatData[18]),
    Z_U_Per(mFloatData[19]),
    Plata_1(mFloatData[20]),
    Plata_2(mFloatData[21]),
    Plata_3(mFloatData[22]),
    Plata_4_5_6(mFloatData[23]),
    Plata_7(mFloatData[24]),
    Plata_8(mFloatData[25]),
    Plata_860mA(mFloatData[26]),
    POS_1_REAL(mFloatData[27]),
    POS_2_REAL(mFloatData[28]),
    POS_3_REAL(mFloatData[29]),
    P_YB3(mFloatData[34]),          /*w*/
    P_YB4(mFloatData[35]),          /*w*/
    Manual_set(mFloatData[36]),     /*w*/
    man_zol_v(mFloatData[37]),      /*w*/
    man_zol_n(mFloatData[38]),      /*w*/
    man_tupe_sign(mBoolData[1])     /*w*/

{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mIntData, 0, sizeof(mIntData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB50", mAdresses, BOOL_COUNT + INT_COUNT + FLOAT_COUNT );
}

void DB50::Read()
{
    std::lock_guard< std::mutex > lock( mLock );
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < BOOL_COUNT + INT_COUNT + FLOAT_COUNT; i++)
    {
        if ( i < BOOL_COUNT )
            mBoolData[ i ] = rez[i].vDataValue.boolVal;
        else if ( i < ( BOOL_COUNT + INT_COUNT ) )
            mIntData[ i - BOOL_COUNT ] = rez[i].vDataValue.lVal;
        else
            mFloatData[ i - ( BOOL_COUNT + INT_COUNT ) ] = rez[i].vDataValue.fltVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

std::mutex& DB50::ReadLocker()
{
    return mLock;
}

void DB50::WriteTask()
{
    HRESULT res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, 1, 1, static_cast<void*>( &man_tupe_sign ), opc::tBOOL );
    res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT + INT_COUNT + FLOAT_COUNT - 9, 9, static_cast<void*>( &YB1 ), opc::tFLOAT );
}

}
}
