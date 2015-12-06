#include "db37.h"
#include "../myOPC/miniOPC.h"

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
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB37", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB37::Read()
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

