#include "db34.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{

DB34::DB34():

	op10_ok ( mBoolData[0]),
	op11_ok ( mBoolData[1]),
	op12_ok ( mBoolData[2]),
	op13_ok ( mBoolData[3]),
	op14_ok ( mBoolData[4]),
	op15_ok ( mBoolData[5]),
	op10_end ( mBoolData[6]),
	op11_end ( mBoolData[7]),
	op12_end ( mBoolData[8]),
	op13_end ( mBoolData[9]),
	op14_end ( mBoolData[10]),
    op15_end ( mBoolData[11]),
    op12_ab_open_a( mBoolData[12]),
    op12_b_close_a( mBoolData[13]),
    op12_a_close_a( mBoolData[14]),
    op12_ab_open_b( mBoolData[15]),
    op12_b_close_b( mBoolData[16]),
    op12_a_close_b( mBoolData[17]),
    op14_ready( mBoolData[18] ),
    op14_frequency( mFloatData[0] )
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB34", mAdresses, BOOL_COUNT + FLOAT_COUNT );
#ifdef DEMO
    op10_ok = true;
    op11_ok = true;
    op12_ok = true;
    op13_ok = true;
    op14_ok = true;
    op15_ok = true;
    op10_end = true;
    op11_end = true;
    op12_end = true;
    op13_end = true;
    op14_end = true;
    op15_end = true;
    HRESULT res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, BOOL_COUNT, FLOAT_COUNT, static_cast<void*>( mFloatData ), opc::tFLOAT );
    res = E_FAIL;
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
#endif
}

void DB34::Read()
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

