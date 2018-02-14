#include "db90.h"

#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
DB90::DB90()
{
    memset( Pressure_S1, 0, sizeof(Pressure_S1) );
    memset( Pressure_S2, 0, sizeof(Pressure_S2) );
    memset( Consumption_S1, 0, sizeof(Consumption_S1) );
    memset( Consumption_S2, 0, sizeof(Consumption_S2) );
    memset( K_Consumption_S1, 0, sizeof(K_Consumption_S1) );
    memset( K_Consumption_S2, 0, sizeof(K_Consumption_S2) );
    memset( KPD, 0, sizeof(KPD) );
    memset( Power, 0, sizeof(Power) );

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

    opc::ReadToArray( rez[0].vDataValue, Pressure_S1, DATA_COUNT);
    opc::ReadToArray( rez[1].vDataValue, Pressure_S2, DATA_COUNT);
    opc::ReadToArray( rez[2].vDataValue, Consumption_S1, DATA_COUNT);
    opc::ReadToArray( rez[3].vDataValue, Consumption_S2, DATA_COUNT);
    opc::ReadToArray( rez[4].vDataValue, Power, DATA_COUNT);
    opc::ReadToArray( rez[5].vDataValue, K_Consumption_S1, DATA_COUNT);
    opc::ReadToArray( rez[6].vDataValue, K_Consumption_S2, DATA_COUNT);
    opc::ReadToArray( rez[7].vDataValue, KPD, DATA_COUNT);


    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}
}
