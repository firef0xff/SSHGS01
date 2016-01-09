#include "db36.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{
DB36::DB36():


    op20_ok( mBoolData[0] ),
    op21_ok( mBoolData[1] ),
    op22_ok( mBoolData[2] ),
    op23_ok( mBoolData[3] ),
    op24_ok( mBoolData[4] ),
    op25_ok( mBoolData[5] ),
    op20_end( mBoolData[6] ),
    op21_end( mBoolData[7] ),
    op22_end( mBoolData[8] ),
    op23_end( mBoolData[9] ),
    op24_end( mBoolData[10] ),
    op25_end( mBoolData[11] ),
    op22_a_ab_open( mBoolData[12] ),
    op22_a_b_close( mBoolData[13] ),
    op22_a_a_close( mBoolData[14] ),
    op22_b_ab_open( mBoolData[15] ),
    op22_b_b_close( mBoolData[16] ),
    op22_b_a_close( mBoolData[17] ),
    op24_ready( mBoolData[18] ),
    op24_frequency( mFloatData[0] )

{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB36", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB36::Read()
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

