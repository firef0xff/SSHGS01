#include "db40.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"
namespace cpu
{
namespace data
{
DB40::DB40():
    sl1( mBoolData[0] ),
    sl2( mBoolData[1] ),
    sl3( mBoolData[2] ),
    sl4( mBoolData[3] ),
    sl5( mBoolData[4] ),
    sp1( mBoolData[5] ),
    sp2( mBoolData[6] ),
    sp3( mBoolData[7] ),
    sp4( mBoolData[8] ),
    sp5( mBoolData[9] ),
    sp6( mBoolData[10] ),
    sp7( mBoolData[11] ),
    sp8( mBoolData[12] ),
    sp9( mBoolData[13] ),
    sp10( mBoolData[14] ),
    sp11( mBoolData[15] ),
    sp12( mBoolData[16] ),
    sp13( mBoolData[17] ),
    sp14( mBoolData[18] ),
    sp15( mBoolData[19] ),
    sq1( mBoolData[20] ),
    sq3( mBoolData[21] ),
    sq5( mBoolData[22] ),
    sq7( mBoolData[23] ),
    sq9( mBoolData[24] ),
    sq11( mBoolData[25] ),
    sq13( mBoolData[26] ),
    sq14( mBoolData[27] ),
    sq15( mBoolData[28] ),
    fault_m1( mBoolData[29] ),
    fault_m2( mBoolData[30] ),
    fault_m3( mBoolData[31] ),
    fault_m4( mBoolData[32] ),
    fault_m5( mBoolData[33] ),
    fault_m6( mBoolData[34] ),
    fault_m8( mBoolData[35] ),
    fault_m10( mBoolData[36] ),
    fault_m12( mBoolData[37] ),
    fault_phasing( mBoolData[38] ),
    p_no( mBoolData[39] ),         //X6.7    давление в системе не установилось
    p_upr_no( mBoolData[40] ),     //X7.0    давление в канале Х не установилось
    q_no( mBoolData[41] ),         //X7.1    расход не установился
    v_no( mBoolData[42] ),         //X7.2    Напряжение не в допуске
    fault_p_op12( mBoolData[43] ), //X7.3    Аварийное давление
    fault_p_op22( mBoolData[44] ) //X7.4    Аварийное давление
{
    memset( mBoolData, 0, sizeof(mBoolData) );
//    memset( mFloatData, 0, sizeof(mFloatData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"DB40", mAdresses, BOOL_COUNT + FLOAT_COUNT );
}

void DB40::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < BOOL_COUNT + FLOAT_COUNT; i++)
    {
        if ( i < BOOL_COUNT )
            mBoolData[ i ] = rez[i].vDataValue.boolVal;
//        else
//            mFloatData[ i - BOOL_COUNT ] = rez[i].vDataValue.fltVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}//namespace data
}//namespace cpu

