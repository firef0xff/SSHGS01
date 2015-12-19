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
    N_Operation( mFloatData[0] ),
    Q_5_5ma ( mFloatData[1] )
{
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB31", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB31::Write()
{
    opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );
    opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
}

}//namespace data
}//namespace cpu

