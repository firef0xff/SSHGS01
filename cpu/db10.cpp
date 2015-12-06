#include "db10.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB10::DB10()
{
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB10", mAdresses, ARRAYS_COUNT );
}

void DB10::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }

    opc::ReadToArray( rez[0].vDataValue, ref,           REF_COUNT         );
    opc::ReadToArray( rez[1].vDataValue, coordinate,    COORDINATE_COUNT );

    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
