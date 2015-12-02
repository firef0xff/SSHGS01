#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{
class CpuMemory;
namespace data
{

//таблица с ответапи по гидроиспытаниям
class DB40 : public In
{
public:

    void Read();

    bool& sl1;  //2.0 уровень масла в норме
    bool& sl2;  //2.1 пониженный уровень масла
    bool& sl3;  //2.2 аварийный уровень масла
    bool& sl4;  //2.3 верхний уровень перелив
    bool& sl5;  //2.4 нижний уровень перелив
    bool& sp1;  //2.5 фильтр всасывающий М1 загрязнен
    bool& sp2;  //2.6 фильтр всасывающий М2 загрязнен
    bool& sp3;  //2.7 фильтр всасывающий М3
    bool& sp4;  //3.0 фильтр всасывающий М3
    bool& sp5;  //3.1 фильтр всасывающий М4 загрязнен
    bool& sp6;  //3.2 фильтр всасывающий М5 загрязнен
    bool& sp7;  //3.3 фильтр тонкой очистки М1 загрязнен
    bool& sp8;  //3.4 фильтр грубой очистки М1 загрязнен
    bool& sp9;  //3.5 фильтр тонкой очистки М2 загрязнен
    bool& sp10;     //3.6 фильтр грубой очистки М2 загрязнен
    bool& sp11;     //3.7 фильтр тонкой очистки М4 загрязнен
    bool& sp12;     //4.0 фильтр грубой очистки М3
    bool& sp13;     //4.1 фильтр грубой очистки М3
    bool& sp14;     //4.2 фильтр контура охлаждения загрязнен
    bool& sp15;     //4.3 фильтр тонкой очистки М12 загрязнен
    bool& sq1;  //4.4 кран М1 закрыт
    bool& sq3;  //4.5 кран М2 закрыт
    bool& sq5;  //4.6 кран М3
    bool& sq7;  //4.7 кран М3
    bool& sq9;  //5.0 кран М4 закрыт
    bool& sq11;     //5.1 кран М5 закрыт
    bool& sq13;     //5.2 кран дренажа М1 закрыт
    bool& sq14;     //5.3 кран дренажа М2 закрыт
    bool& sq15;     //5.4 кран дренажа М4 закрыт
    bool& fault_m1;     //5.5 нет включения М1
    bool& fault_m2;     //5.6 нет включения М2
    bool& fault_m3;     //5.7 нет включения М3
    bool& fault_m4;     //6.0 нет включения М4
    bool& fault_m5;     //6.1 нет включения М5
    bool& fault_m6;     //6.2 нет включения М6
    bool& fault_m8;     //6.3 нет включения М8
    bool& fault_m10;    //6.4 нет включения М10
    bool& fault_m12;    //6.5 нет включения М12
    bool& fault_phasing;    //6.6 не правильная фазировка

private:

    friend class cpu::CpuMemory;
    DB40();
    DB40( const DB40& ) = delete;
    void operator = ( const DB40& ) = delete;

    enum
    {
        BOOL_COUNT = 39,
        FLOAT_COUNT = 0
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        L"CPU/DB40.sl1",
        L"CPU/DB40.sl2",
        L"CPU/DB40.sl3",
        L"CPU/DB40.sl4",
        L"CPU/DB40.sl5",
        L"CPU/DB40.sp1",
        L"CPU/DB40.sp2",
        L"CPU/DB40.sp3",
        L"CPU/DB40.sp4",
        L"CPU/DB40.sp5",
        L"CPU/DB40.sp6",
        L"CPU/DB40.sp7",
        L"CPU/DB40.sp8",
        L"CPU/DB40.sp9",
        L"CPU/DB40.sp10",
        L"CPU/DB40.sp11",
        L"CPU/DB40.sp12",
        L"CPU/DB40.sp13",
        L"CPU/DB40.sp14",
        L"CPU/DB40.sp15",
        L"CPU/DB40.sq1",
        L"CPU/DB40.sq3",
        L"CPU/DB40.sq5",
        L"CPU/DB40.sq7",
        L"CPU/DB40.sq9",
        L"CPU/DB40.sq11",
        L"CPU/DB40.sq13",
        L"CPU/DB40.sq14",
        L"CPU/DB40.sq15",
        L"CPU/DB40.fault_m1",
        L"CPU/DB40.fault_m2",
        L"CPU/DB40.fault_m3",
        L"CPU/DB40.fault_m4",
        L"CPU/DB40.fault_m5",
        L"CPU/DB40.fault_m6",
        L"CPU/DB40.fault_m8",
        L"CPU/DB40.fault_m10",
        L"CPU/DB40.fault_m12",
        L"CPU/DB40.fault_phasing"
    };
};

}//namespace data

}//namespace cpu
