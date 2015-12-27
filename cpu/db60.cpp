#include "db60.h"
#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{

DB60::DB60():
    OP15_25_Opor_1(mBoolData[0]),
    OP15_25_Opor_2(mBoolData[1]),
    OP15_25_Opor_3(mBoolData[2]),    
    OP15_25_count ( mIntData[0])
{
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB60", mAdresses, BOOL_COUNT + INT_COUNT );
}

void DB60::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < BOOL_COUNT + INT_COUNT; i++)
    {
        if ( i < BOOL_COUNT )
            mBoolData[ i ] = rez[i].vDataValue.boolVal;
        else
            mIntData[ i - BOOL_COUNT ] = rez[i].vDataValue.lVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}//namespace data
}//namespace cpu

