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

    auto ReadData = []( OPCITEMSTATE& result, float* array, uint32_t size )
    {
        float* values = nullptr;
        opc::miniOPC::Instance().GetArrayData( result.vDataValue, reinterpret_cast<void**>(&values) );
        for (size_t i = 0; i < size; i++)
        {
            array[ i ] = values[i];
        }
        opc::miniOPC::Instance().FreeArrayData( result.vDataValue );
    };

    ReadData( rez[0], ref_a,            REF_A_COUNT         );
    ReadData( rez[1], consumption_a,    CONSUMPTION_A_COUNT );
    ReadData( rez[2], ref_b,            REF_B_COUNT         );
    ReadData( rez[3], consumption_b,    CONSUMPTION_B_COUNT );

    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}
void DB2::Write()
{

}

}
}
