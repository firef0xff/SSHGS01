#include "db2.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB2::DB2()
{
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB2", mAdresses, ARRAYS_COUNT );
}

void DB2::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    float* values = nullptr;
    SafeArrayAccessData(res[0].vDataValue.parray, (void **)& values);
    for (size_t i = 0; i < REF_A_COUNT; i++)
    {
        ref_a[ i ] = values[i];
    }
    SafeArrayUnaccessData(res[0].vDataValue.parray);

    SafeArrayAccessData(res[1].vDataValue.parray, (void **)& values);
    for (size_t i = 0; i < CONSUMPTION_A_COUNT; i++)
    {
        consumption_a[ i ] = values[i];
    }
    SafeArrayUnaccessData(res[0].vDataValue.parray);

    SafeArrayAccessData(res[0].vDataValue.parray, (void **)& values);
    for (size_t i = 0; i < REF_B_COUNT; i++)
    {
        ref_b[ i ] = values[i];
    }
    SafeArrayUnaccessData(res[0].vDataValue.parray);

    SafeArrayAccessData(res[0].vDataValue.parray, (void **)& values);
    for (size_t i = 0; i < CONSUMPTION_B_COUNT; i++)
    {
        consumption_b[ i ] = values[i];
    }
    SafeArrayUnaccessData(res[0].vDataValue.parray);

    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}
void DB2::Write()
{

}

}
}
