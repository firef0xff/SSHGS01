#include "m1.h"
#include "../myOPC/miniOPC.h"
#include "memory.h"

namespace cpu
{
namespace data
{
M1::M1():
    YA1(mBoolData[0]),          //MX40.0 Разгрузка управл.насоса для М1 м3
    YA2(mBoolData[1]),          //MX40.1 Разгрузка управл.насоса для М2 м3
    YA3(mBoolData[2]),          //MX40.2 Разгрузка насоса управления м4
    YA4(mBoolData[3]),          //MX40.3 Включение гидроаккумулятора
    YA5(mBoolData[4]),          //MX40.4 Разгрузка основных насосов м1м2
    YA6(mBoolData[5]),          //MX40.5 Подача расхода к испытательному модулю
    YA7(mBoolData[6]),          //MX40.6 bnbmВключ малый расходомер (до 60л/мин)
    YA8(mBoolData[7]),          //MX40.7 Закрытие канала А
    YA9(mBoolData[8]),          //MX41.0 Закрытие канала В
    YA10(mBoolData[9]),         //MX41.1 Закрытие канала Т
    YA11(mBoolData[10]),         //MX41.2 Подача масла к расходомерам из канала Т
    YA12(mBoolData[11]),         //MX41.3 Подача масла к расходомерам из канала У
    YA13(mBoolData[12]),         //MX41.4 Декомпрессия канала Р
    YA14(mBoolData[13]),         //MX41.5 Декомпрессия канала Т
    YA15(mBoolData[14]),         //MX41.6 Декомпрессия канала А
    YA16(mBoolData[15]),         //MX41.7 Декомпрессия канала В
    YA17(mBoolData[16]),         //MX42.0 Декомпрессия канала Х
    YA18(mBoolData[17]),         //MX42.1 Включение г/цилиндра 1 (1-10 л/мин) испытания частоты канала А
    YA19(mBoolData[18]),         //MX42.2 Включение г/цилиндра 1 (1-10 л/мин) испытания частоты канала В
    YA20(mBoolData[19]),         //MX42.3 Включение г/цилиндра 2 (0-1 л/мин) испытания канала А
    YA21(mBoolData[20]),         //MX42.4 Включение г/цилиндра 2 (0-1 л/мин) испытания канала В
    YA22(mBoolData[21]),         //MX42.5 Открытие канала Х
    YA23(mBoolData[22]),         //MX42.6 Включить малый расходомер утечек
    YA24(mBoolData[23]),         //MX42.7 Подвод г/цилиндра
    YA25(mBoolData[24]),         //MX43.0 Отвод г/цилиндра
    KM1(mBoolData[25]),          //MX43.1 Насос М1
    KM2(mBoolData[26]),          //MX43.2 Насос М2
    KM3(mBoolData[27]),          //MX43.3 Насос М3
    KM4(mBoolData[28]),          //MX43.4 Насос М4
    KM5(mBoolData[29]),          //MX43.5 Насос контрура охлаждения М5
    KM9(mBoolData[30]),          //MX43.6 Насос М12
    KM6(mBoolData[31]),          //MX43.7 Вентилятор М6,М7
    KM7(mBoolData[32]),          //MX44.0 Вентилятор М8,М9
    KM8(mBoolData[33]),          //MX44.1 Вентилятор М10,М11
    KM10(mBoolData[34]),         //MX44.2 ТЭНы
    Err_clear(mBoolData[35]),    //MX44.7 сброс не правильной комбинации...
    ONRA(mBoolData[36]),         //MX45.2 ВКЛ катушку А
    ONRB(mBoolData[37]),         //MX45.3 ВКЛ катушку В
    CB(mBoolData[38]),           //MX45.4 Управление от карты
    CR(mBoolData[39]),           //MX45.5 управление без карты
    RC1(mBoolData[40]),          //MX45.6 1 катушка
    RC2(mBoolData[41])           //MX45.7 2 катушки
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    mGroupID = opc::miniOPC::Instance().AddGroup( L"M1", mAdresses, BOOL_COUNT );
}

void M1::Reset()
{
    memset( mBoolData, 0, sizeof(mBoolData) );
    auto res = opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
    while ( res == E_FAIL )
        opc::miniOPC::Instance().WriteMass( mGroupID, 0, BOOL_COUNT, static_cast<void*>( mBoolData ), opc::tBOOL );
}

void M1::Read()
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

void M1::SendBit( bool& val, int pos )
{
    auto res = opc::miniOPC::Instance().WriteValue( mGroupID, pos, static_cast<void*>( &val ), opc::tBOOL );
    while ( res == E_FAIL )
        res = opc::miniOPC::Instance().WriteValue( mGroupID, pos, static_cast<void*>( &val ), opc::tBOOL );
}

void M1::SetYA1( bool v )          //MX40.0 Разгрузка управл.насоса для М1 м3
{
    YA1 = v;
    SendBit( YA1, 0 );
}
void M1::SetYA2( bool v )          //MX40.1 Разгрузка управл.насоса для М2 м3
{
    YA2 = v;
    SendBit( YA2, 1 );
}
void M1::SetYA3( bool v )          //MX40.2 Разгрузка насоса управления м4
{
    YA3 = v;
    SendBit( YA3, 2 );
}
void M1::SetYA4( bool v )          //MX40.3 Включение гидроаккумулятора
{
    YA4 = v;
    SendBit( YA4, 3 );
}
void M1::SetYA5( bool v )          //MX40.4 Разгрузка основных насосов м1м2
{
    YA5 = v;
    SendBit( YA5, 4 );
}
void M1::SetYA6( bool v )          //MX40.5 Подача расхода к испытательному модулю
{
    YA6 = v;
    SendBit( YA6, 5 );
}
void M1::SetYA7( bool v )          //MX40.6 bnbmВключ малый расходомер (до 60л/мин)
{
    YA7 = v;
    SendBit( YA7, 6 );
}
void M1::SetYA8( bool v )          //MX40.7 Закрытие канала А
{
    YA8 = v;
    SendBit( YA8, 7 );
}
void M1::SetYA9( bool v )          //MX41.0 Закрытие канала В
{
    YA9 = v;
    SendBit( YA9, 8 );
}
void M1::SetYA10( bool v )         //MX41.1 Закрытие канала Т
{
    YA10 = v;
    SendBit( YA10, 9 );
}
void M1::SetYA11( bool v )         //MX41.2 Подача масла к расходомерам из канала Т
{
    YA11 = v;
    SendBit( YA11, 10 );
}
void M1::SetYA12( bool v )         //MX41.3 Подача масла к расходомерам из канала У
{
    YA12 = v;
    SendBit( YA12, 11 );
}
void M1::SetYA13( bool v )         //MX41.4 Декомпрессия канала Р
{
    YA13 = v;
    SendBit( YA13, 12 );
}
void M1::SetYA14( bool v )         //MX41.5 Декомпрессия канала Т
{
    YA14 = v;
    SendBit( YA14, 13 );
}
void M1::SetYA15( bool v )         //MX41.6 Декомпрессия канала А
{
    YA15 = v;
    SendBit( YA15, 14 );
}
void M1::SetYA16( bool v )         //MX41.7 Декомпрессия канала В
{
    YA16 = v;
    SendBit( YA16, 15 );
}
void M1::SetYA17( bool v )         //MX42.0 Декомпрессия канала Х
{
    YA17 = v;
    SendBit( YA17, 16 );
}
void M1::SetYA18( bool v )         //MX42.1 Включение г/цилиндра 1 (1-10 л/мин) испытания частоты канала А
{
    YA18 = v;
    SendBit( YA18, 17 );
}
void M1::SetYA19( bool v )         //MX42.2 Включение г/цилиндра 1 (1-10 л/мин) испытания частоты канала В
{
    YA19 = v;
    SendBit( YA19, 18 );
}
void M1::SetYA20( bool v )         //MX42.3 Включение г/цилиндра 2 (0-1 л/мин) испытания канала А
{
    YA20 = v;
    SendBit( YA20, 19 );
}
void M1::SetYA21( bool v )         //MX42.4 Включение г/цилиндра 2 (0-1 л/мин) испытания канала В
{
    YA21 = v;
    SendBit( YA21, 20 );
}
void M1::SetYA22( bool v )         //MX42.5 Открытие канала Х
{
    YA22 = v;
    SendBit( YA22, 21 );
}
void M1::SetYA23( bool v )         //MX42.6 Включить малый расходомер утечек
{
    YA23 = v;
    SendBit( YA23, 22 );
}
void M1::SetYA24( bool v )         //MX42.7 Подвод г/цилиндра
{
    YA24 = v;
    SendBit( YA24, 23 );
}
void M1::SetYA25( bool v )         //MX43.0 Отвод г/цилиндра
{
    YA25 = v;
    SendBit( YA25, 24 );
}
void M1::SetKM1( bool v )          //MX43.1 Насос М1
{
    KM1 = v;
    SendBit( KM1, 25 );
}
void M1::SetKM2( bool v )          //MX43.2 Насос М2
{
    KM2 = v;
    SendBit( KM2, 26 );
}
void M1::SetKM3( bool v )          //MX43.3 Насос М3
{
    KM3 = v;
    SendBit( KM3, 27 );
}
void M1::SetKM4( bool v )          //MX43.4 Насос М4
{
    KM4 = v;
    SendBit( KM4, 28 );
}
void M1::SetKM5( bool v )          //MX43.5 Насос контрура охлаждения М5
{
    KM5 = v;
    SendBit( KM5, 29 );
}
void M1::SetKM9( bool v )          //MX43.6 Насос М12
{
    KM9 = v;
    SendBit( KM9, 30 );
}
void M1::SetKM6( bool v )          //MX43.7 Вентилятор М6,М7
{
    KM6 = v;
    SendBit( KM6, 31 );
}
void M1::SetKM7( bool v )          //MX44.0 Вентилятор М8,М9
{
    KM7 = v;
    SendBit( KM7, 32 );
}
void M1::SetKM8( bool v )          //MX44.1 Вентилятор М10,М11
{
    KM8 = v;
    SendBit( KM8, 33 );
}
void M1::SetKM10( bool v )         //MX44.2 ТЭНы
{
    KM10 = v;
    SendBit( KM10, 34 );
}
void M1::SetErr_clear()    //MX44.7 сброс не правильной комбинации...
{
    Err_clear = true;
    SendBit( Err_clear, 35 );
}
void M1::SetONRA( bool v )         //MX45.2 ВКЛ катушку А
{
    ONRA = v;
    SendBit( ONRA, 36 );
}
void M1::SetONRB( bool v )         //MX45.3 ВКЛ катушку В
{
    ONRB = v;
    SendBit( ONRB, 37 );
}
void M1::SetCB( bool v )           //MX45.4 Управление от карты
{
    CB = v;
    SendBit( CB, 38 );
}
void M1::SetCR( bool v )           //MX45.5 управление без карты
{
    CR = v;
    SendBit( CR, 39 );
}
void M1::SetRC1( bool v )          //MX45.6 1 катушка
{
    RC1 = v;
    SendBit( RC1, 40 );
}
void M1::SetRC2( bool v )          //MX45.7 2 катушки
{
    RC2 = v;
    SendBit( RC2, 41 );
}

}//namespace data
}//namespace cpu

