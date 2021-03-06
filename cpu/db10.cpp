#include "db10.h"

#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB10::DB10()
{
    memset( signal, 0, sizeof(signal) );
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

    opc::ReadToArray( rez[0].vDataValue, signal, SIGNAL_COUNT);
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
