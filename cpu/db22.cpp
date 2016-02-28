#include "db22.h"

#include "../myOPC/miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB22::DB22()
{
    memset( signal, 0, sizeof(signal) );
    memset( current, 0, sizeof(current) );
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

    opc::ReadToArray( rez[0].vDataValue, signal,   SIGNAL_COUNT );
    opc::ReadToArray( rez[1].vDataValue, current,  CURRENT_COUNT );

    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );

}

}
}
