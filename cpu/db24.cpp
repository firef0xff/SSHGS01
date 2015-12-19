#include "db24.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB24::DB24()
{
//    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB24", mAdresses, ARRAYS_COUNT );
}

void DB24::Read()
{
//    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
//    if (!rez)
//    {
//        //ошибка подключения..
//        return;
//    }

//    opc::ReadToArray( rez[0].vDataValue, coordinate, COORDINATE_COUNT );
    
//    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
