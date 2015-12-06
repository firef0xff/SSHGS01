#include "db39.h"
#include "../myOPC/miniOPC.h"

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
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB39", mAdresses, BOOL_COUNT + FLOAT_COUNT );
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
        else
            mFloatData[ i - BOOL_COUNT ] = rez[i].vDataValue.fltVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}//namespace data
}//namespace cpu

