#include "i1.h"
#include "miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{

I1::I1():
    SQ1( mBoolData[0] ),     //IX0.0    MXкран насоса М1
    SQ3( mBoolData[1] ),     //IX0.2    MXкран насоса М2
    SQ5( mBoolData[2] ),     //IX0.4    MXкран насоса М3.2
    SQ7( mBoolData[3] ),     //IX0.6    MXкран насоса М3.1
    SQ9( mBoolData[4] ),     //IX1.0    MXкран насоса М4
    SQ11( mBoolData[5] ),    //IX1.2    MXкран насоса М5
    SQ13( mBoolData[6] ),    //IX1.4    MXкран дренажа М1
    SQ14( mBoolData[7] ),    //IX1.5    MXкран дренажа М2
    SQ15( mBoolData[8] ),    //IX1.6    MXкран дренажа М4
    SP1( mBoolData[9] ),     //IX2.0    MXфильтр всасывающий М1
    SP2( mBoolData[10] ),    //IX2.1    MXфильтр всасывающий М2
    SP3( mBoolData[11] ),    //IX2.2    MXфильтр всасывающий М3.2
    SP4( mBoolData[12] ),    //IX2.3    MXфильтр всасывающий М3.1
    SP5( mBoolData[13] ),    //IX2.4    MXфильтр всасывающий М4
    SP6( mBoolData[14] ),    //IX2.5    MXфильтр всасывающий М5
    SP7( mBoolData[15] ),    //IX2.6    MXфильтр тонкой очистки М1
    SP8( mBoolData[16] ),    //IX2.7    MXфильтр грубой очистки М1
    SP9( mBoolData[17] ),    //IX3.0    MXфильтр тонкой очистки М2
    SP10( mBoolData[18] ),   //IX3.1    MXфильтр грубой очистки М2
    SP11( mBoolData[19] ),   //IX3.2    MXфильтр тонкой очистки М4
    SP12( mBoolData[20] ),   //IX3.3    MXфильтр тонкой очистки М3.2
    SP13( mBoolData[21] ),   //IX3.4    MXфильтр тонкой очистки М3.1
    SP14( mBoolData[22] ),   //IX3.5    MXфильтр контура охлаждения
    SP15( mBoolData[23] ),   //IX3.6    MXфильтр тонкой очистки М12
    SL1( mBoolData[24] ),    //IX4.0    MXуровень масла - норма
    SL2( mBoolData[25] ),    //IX4.1    MXуровень масла -  предупреждение
    SL3( mBoolData[26] ),    //IX4.2    MXуровень масла - авария
    SL4( mBoolData[27] ),    //IX4.3    MXуровень масла верхний (поддон)
    SL5( mBoolData[28] ),    //IX4.4    MXуровень масла нижний (поддон)
    Error( mBoolData[29] )   //MX44.6    MXне правильная комбинация вкл клапанов
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"I1", mAdresses, BOOL_COUNT );
}

void I1::Read()
{
    OPCITEMSTATE* rez = opc::miniOPC::Instance().Read( mGroupID );
    if (!rez)
    {
        //ошибка подключения..
        return;
    }
    for (size_t i = 0; i < BOOL_COUNT; i++)
    {
        if ( i < BOOL_COUNT )
            mBoolData[ i ] = rez[i].vDataValue.boolVal;
    }
    opc::miniOPC::Instance().OpcMassFree( mGroupID, rez );
}

}//namespace data
}//namespace cpu
