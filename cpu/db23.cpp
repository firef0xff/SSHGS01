#include "db23.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB23::DB23()
{
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
