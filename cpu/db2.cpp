#include "db2.h"

#include "miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{
DB2::DB2()
{
    memset( ref_a, 0, sizeof(ref_a) );
    memset( consumption_a, 0, sizeof(consumption_a) );
    memset( ref_b, 0, sizeof(ref_b) );
    memset( consumption_b, 0, sizeof(consumption_b) );
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

    opc::ReadToArray( rez[0].vDataValue, ref_a,            REF_A_COUNT         );
    opc::ReadToArray( rez[1].vDataValue, consumption_a,    CONSUMPTION_A_COUNT );
    opc::ReadToArray( rez[2].vDataValue, ref_b,            REF_B_COUNT         );
    opc::ReadToArray( rez[3].vDataValue, consumption_b,    CONSUMPTION_B_COUNT );

    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
