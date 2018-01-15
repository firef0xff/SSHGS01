#include "db90.h"

#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB90::DB90()
{
    memset( Pressure, 0, sizeof(Pressure) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB90", mAdresses, ARRAYS_COUNT );
}

void DB90::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }

    opc::ReadToArray( rez[0].vDataValue, Pressure, DATA_COUNT);
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
