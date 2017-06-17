#include "db9.h"

#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB9::DB9()
{
    memset( consumption_a, 0, sizeof(consumption_a) );
    memset( bp3_a, 0, sizeof(bp3_a) );
    memset( bp5_a, 0, sizeof(bp5_a) );
    memset( consumption_b, 0, sizeof(consumption_b) );
    memset( bp4_b, 0, sizeof(bp4_b) );
    memset( bp5_b, 0, sizeof(bp5_b) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB9", mAdresses, ARRAYS_COUNT );
}

void DB9::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }

    opc::ReadToArray( rez[0].vDataValue, consumption_a,  CONSUMPTION_A_COUNT         );
    opc::ReadToArray( rez[1].vDataValue, bp3_a,          BP3_A_COUNT );
    opc::ReadToArray( rez[2].vDataValue, bp5_a,          BP5_A_COUNT         );
    opc::ReadToArray( rez[3].vDataValue, consumption_b,  CONSUMPTION_B_COUNT );
    opc::ReadToArray( rez[4].vDataValue, bp4_b,          BP4_B_COUNT         );
    opc::ReadToArray( rez[5].vDataValue, bp5_b,          BP5_B_COUNT );

    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
