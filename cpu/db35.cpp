#include "db35.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{
DB35::DB35():

    channel_a( mBoolData[0] ),
    channel_b( mBoolData[1] ),
    channel_k_a( mBoolData[2] ),
    channel_k_b( mBoolData[3] ),


    s860ma( mFloatData[0] ),
    x_max_a( mFloatData[1] ),
    x_max_b( mFloatData[2] ),
    x_pos_0( mFloatData[3] ),
    test_press( mFloatData[4] ),
    nominal_press( mFloatData[5] ),
    q_max_a( mFloatData[6] ),
    q_max_b( mFloatData[7] ),
    q_max( mFloatData[8] ),
    amp_1( mFloatData[9] ),
    amp_2( mFloatData[10] ),
    amp_3( mFloatData[11] ),
    increment( mFloatData[12] ),
    press_control_min( mFloatData[13] ),
    press_control_max( mFloatData[14] )
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB35", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB35::Reset()
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    Write();
}

void DB35::Write()
{
    HRESULT res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );
    res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
}

}//namespace data
}//namespace cpu

