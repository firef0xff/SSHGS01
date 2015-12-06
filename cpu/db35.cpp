#include "db35.h"
#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB35::DB35():

    channel_a( mBoolData[0] ),
    channel_b( mBoolData[1] ),

    s860ma( mFloatData[0] ),
    x_max_a( mFloatData[1] ),
    x_max_b( mFloatData[2] ),
    x_pos_0( mFloatData[3] ),
    test_press( mFloatData[4] ),
    nominal_press( mFloatData[5] ),
    q_max_a( mFloatData[6] ),
    q_max_b( mFloatData[7] ),
    q_max( mFloatData[8] ),
    amp_1( mFloatData[9] ),
    amp_2( mFloatData[10] ),
    amp_3( mFloatData[11] ),
    increment( mFloatData[12] ),
    press_control_min( mFloatData[13] ),
    press_control_max( mFloatData[14] )
{
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB35", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB35::Read()
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

