#include "db33.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{

DB33::DB33():
    s4_20ma ( mBoolData[0]),
	s0_20ma ( mBoolData[1]),
	s10v ( mBoolData[2]),
    s0_10v( mBoolData[3]),
    s10ma ( mBoolData[4]),
    s15ma ( mBoolData[5]),
    s20ma ( mBoolData[6]),
    s40ma ( mBoolData[7]),
    channel_a ( mBoolData[8] ),
    channel_b ( mBoolData[9] ),
    channel_k_a( mBoolData[10] ),
    channel_k_b( mBoolData[11] ),

	x_max_a ( mFloatData[0] ),
	x_max_b ( mFloatData[1] ),
	x_pos_0 ( mFloatData[2] ),
	test_press ( mFloatData[3] ),
	nominal_press ( mFloatData[4] ),
	q_max_a ( mFloatData[5] ),
	q_max_b ( mFloatData[6] ),
	q_max ( mFloatData[7] ),
	amp_1 ( mFloatData[8] ),
	amp_2 ( mFloatData[9] ),
	amp_3 ( mFloatData[10] ),
	increment ( mFloatData[11] ),
	press_control_min ( mFloatData[12] ),
    press_control_max ( mFloatData[13] ),
    U_Plat( mFloatData[14] ),
    start_frequency( mFloatData[15] ),
    ampl_inc( mFloatData[16] )
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB33", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB33::Reset()
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    Write();
}
void DB33::Write()
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

