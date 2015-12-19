#include "db2.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB2::DB2()
{
//    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB2", mAdresses, ARRAYS_COUNT );
}

void DB2::Read()
{
//    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
//    if (!rez)
//    {
//        //ошибка подключения..
//        return;
//    }

//    opc::ReadToArray( rez[0].vDataValue, ref_a,            REF_A_COUNT         );
//    opc::ReadToArray( rez[1].vDataValue, consumption_a,    CONSUMPTION_A_COUNT );
//    opc::ReadToArray( rez[2].vDataValue, ref_b,            REF_B_COUNT         );
//    opc::ReadToArray( rez[3].vDataValue, consumption_b,    CONSUMPTION_B_COUNT );

//    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
