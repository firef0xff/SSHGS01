#include "db37.h"
#include "miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{
DB37::DB37():

    s4_20ma( mBoolData[0] ),
    s0_20ma( mBoolData[1] ),
    s10v( mBoolData[2] ),
    s10ma( mBoolData[3] ),
    s15ma( mBoolData[4] ),
    s20ma( mBoolData[5] ),
    s40ma( mBoolData[6] ),

    nominal_volt( mFloatData[0] ),
    output_current_max( mFloatData[1] ),
    resistance( mFloatData[2] )
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB37", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}
void DB37::Reset()
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    Write();
}
void DB37::Write()
{
    HRESULT res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
    res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );
}

}//namespace data
}//namespace cpu

