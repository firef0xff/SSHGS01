#include "db17.h"

#include "../myOPC/miniOPC.h"

namespace cpu
{
namespace data
{
DB17::DB17()
{
//    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB17", mAdresses, ARRAYS_COUNT );
}

void DB17::Read()
{
//    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
//    if (!rez)
//    {
//        //ошибка подключения..
//        return;
//    }

//    opc::ReadToArray( rez[0].vDataValue, open_ref,          OPEN_REF_COUNT );
//    opc::ReadToArray( rez[1].vDataValue, open_consumption,  OPEN_CONSUMPTION_COUNT );
//    opc::ReadToArray( rez[2].vDataValue, open_bp3,          OPEN_BP3_COUNT );
//    opc::ReadToArray( rez[3].vDataValue, open_bp4,          OPEN_BP4_COUNT );
//    opc::ReadToArray( rez[4].vDataValue, open_bp5,          OPEN_BP5_COUNT );
//    opc::ReadToArray( rez[5].vDataValue, close_ref,         CLOSE_REF_COUNT );
//    opc::ReadToArray( rez[6].vDataValue, close_consumption, CLOSE_CONSUMPTION_COUNT );
//    opc::ReadToArray( rez[7].vDataValue, close_bp3,         CLOSE_BP3_COUNT );
//    opc::ReadToArray( rez[8].vDataValue, close_bp4,         CLOSE_BP4_COUNT );
//    opc::ReadToArray( rez[9].vDataValue, close_bp5,         CLOSE_BP5_COUNT );

//    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );

}

}
}
