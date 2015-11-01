#include "db31.h"
#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{

DB31::DB31():
    Start_Oper( mBoolData[0] ),
    Stop_Oper( mBoolData[1] ),
    Nasos_M2( mBoolData[2] ),
    N_Operation( mFloatData[0] )
{
    mGroupID = OPC->AddGroup( L"DB31", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB31::Write()
{
    OPC->WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), tBOOL );
    OPC->WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), tFLOAT );
}

}//namespace data
}//namespace cpu

