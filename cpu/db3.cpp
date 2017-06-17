#include "db3.h"

#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB3::DB3()
{
    memset( open_ref, 0, sizeof(open_ref) );
    memset( open_consumption, 0, sizeof(open_consumption) );
    memset( close_ref, 0, sizeof(close_ref) );
    memset( close_consumption, 0, sizeof(close_consumption) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB3", mAdresses, ARRAYS_COUNT );
}

void DB3::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }

    opc::ReadToArray( rez[0].vDataValue, open_ref,          OPEN_REF_COUNT );
    opc::ReadToArray( rez[1].vDataValue, open_consumption,  OPEN_CONSUMPTION_COUNT );
    opc::ReadToArray( rez[2].vDataValue, close_ref,         CLOSE_REF_COUNT );
    opc::ReadToArray( rez[3].vDataValue, close_consumption, CLOSE_CONSUMPTION_COUNT );

    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );

}

}
}
