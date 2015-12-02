#include "db22.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB22::DB22()
{
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB22", mAdresses, ARRAYS_COUNT );
}

void DB22::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }

    opc::ReadToArray( rez[0].vDataValue, ref,               REF_COUNT         );
    opc::ReadToArray( rez[1].vDataValue, output_current,    OUTPUT_CURRENT_COUNT );

    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
