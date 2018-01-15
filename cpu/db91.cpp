#include "db91.h"

#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB91::DB91()
{
    memset( Value, 0, sizeof(Value) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB10", mAdresses, ARRAYS_COUNT );
}

void DB91::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }

    opc::ReadToArray( rez[0].vDataValue, Value, DATA_COUNT);
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
