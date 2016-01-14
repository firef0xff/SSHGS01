#include "db31.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{

DB31::DB31():
    Start_Oper( mBoolData[0] ),
    Stop_Oper( mBoolData[1] ),
    Nasos_M2( mBoolData[2] ),
    OP15_25_Continum(mBoolData[3]),
    Next_Amp( mBoolData[4] ),
    N_Operation( mFloatData[0] ),
    Q_5_5ma ( mFloatData[1] )
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB31", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB31::Write()
{
    opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );
    opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
}

void DB31::SendContinue()
{
    OP15_25_Continum = true;
    auto res = opc::miniOPC::Instance().WriteValue( mGroupID, 3, static_cast<void*>( &OP15_25_Continum ), opc::tBOOL );
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteValue( mGroupID, 3, static_cast<void*>( &OP15_25_Continum ), opc::tBOOL );
}

void DB31::SendNextAmp()
{
    Next_Amp = true;
    auto res = opc::miniOPC::Instance().WriteValue( mGroupID, 4, static_cast<void*>( &Next_Amp ), opc::tBOOL );
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteValue( mGroupID, 4, static_cast<void*>( &Next_Amp ), opc::tBOOL );
}

}//namespace data
}//namespace cpu

