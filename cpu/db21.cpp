#include "db21.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB21::DB21()
{
//    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB21", mAdresses, ARRAYS_COUNT );
}

void DB21::Read()
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
