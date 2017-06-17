#include "db23.h"

#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB23::DB23()
{
    memset( coordinate, 0, sizeof(coordinate) );

    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB23", mAdresses, ARRAYS_COUNT );
}

void DB23::Read()
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
