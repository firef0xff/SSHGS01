#include "db11.h"

#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB11::DB11()
{    
    memset( coordinate, 0, sizeof(coordinate) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB11", mAdresses, ARRAYS_COUNT );
}

void DB11::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }

    opc::ReadToArray( rez[0].vDataValue, coordinate, COORDINATE_COUNT );
    
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
