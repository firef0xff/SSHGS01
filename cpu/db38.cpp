#include "db38.h"
#include "../myOPC/miniOPC.h"

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
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB38", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB38::Read()
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

