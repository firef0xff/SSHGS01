#include "db38.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{
DB38::DB38():
    p_max( mFloatData[0] ),
    p_function( mFloatData[1] ),
    q_nominal( mFloatData[2] ),
    time_function( mFloatData[3] ),
    time_tightness( mFloatData[4] )
{
//    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB38", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB38::Write()
{
    opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );
}

}//namespace data
}//namespace cpu

