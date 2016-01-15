#include "db39.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB39::DB39():
    op30_ok( mBoolData[0] ),
    op31_ok( mBoolData[1] ),
    op32_ok( mBoolData[2] ),
    op30_end( mBoolData[3] ),
    op31_end( mBoolData[4] ),
    op32_end( mBoolData[5] )
{
    memset( mBoolData, 0, sizeof(mBoolData) );
//    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB39", mAdresses, BOOL_COUNT + FLOAT_COUNT );
#ifdef DEMO
    op30_ok = true;
    op31_ok = true;
    op32_ok = true;
    op30_end = true;
    op31_end = true;
    op32_end = true;
    HRESULT res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
#endif
}

void DB39::Read()
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
//        else
//            mFloatData[ i - BOOL_COUNT ] = rez[i].vDataValue.fltVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}//namespace data
}//namespace cpu

