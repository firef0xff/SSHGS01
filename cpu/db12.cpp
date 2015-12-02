#include "db12.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB12::DB12()
{
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB12", mAdresses, ARRAYS_COUNT );
}

void DB12::Read()
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
