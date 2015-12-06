#include "db33.h"
#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{

DB33::DB33():
    s4_20ma ( mBoolData[0]),
	s0_20ma ( mBoolData[1]),
	s10v ( mBoolData[2]),
	s10ma ( mBoolData[3]),
	s15ma ( mBoolData[4]),
	s20ma ( mBoolData[5]),
	s40ma ( mBoolData[6]),
	channel_a ( mBoolData[7]),
	channel_b ( mBoolData[8]),

	x_max_a ( mFloatData[0] ),
	x_max_b ( mFloatData[1] ),
	x_pos_0 ( mFloatData[2] ),
	test_press ( mFloatData[3] ),
	nominal_press ( mFloatData[4] ),
	q_max_a ( mFloatData[5] ),
	q_max_b ( mFloatData[6] ),
	q_max ( mFloatData[7] ),
	amp_1 ( mFloatData[8] ),
	amp_2 ( mFloatData[9] ),
	amp_3 ( mFloatData[10] ),
	increment ( mFloatData[11] ),
	press_control_min ( mFloatData[12] ),
	press_control_max ( mFloatData[13] )
{
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB33", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB33::Read()
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

