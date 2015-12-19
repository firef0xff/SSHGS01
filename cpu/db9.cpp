#include "db9.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB9::DB9()
{
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
