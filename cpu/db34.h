#pragma once
#include "data_block_base.h"
#include <inttypes.h>

namespace cpu
{

class CpuMemory;

namespace data
{

class DB34 :public Out
{
public:

    void Read();

    bool& op10_ok; //2.0 наружняя герметичность выполнение
    bool& op11_ok; //2.1 внутренняя герметичность выполнение
    bool& op12_ok; //2.2 зависимоть расход/ток выполнение
    bool& op13_ok; //2.3 зависимость расход/давление выполнение
    bool& op14_ok; //2.4 частотные характеристики выполнение
    bool& op15_ok; //2.5 переходные харарактеристики выполнение
    bool& op10_end; //2.6 наружняя герметичность конец
    bool& op11_end; //2.7 внутренняя герметичность конец
    bool& op12_end; //3.0 зависимоть расход/ток конец
    bool& op13_end; //3.1 зависимость расход/давление конец
    bool& op14_end; //3.2 частотные характеристики конец
    bool& op15_end; //3.3 переходные харарактеристики конец

private:
    friend class cpu::CpuMemory;
    DB34();
    DB34( const DB34& ) = delete;
    void operator = ( const DB34& ) = delete;

    enum
    {
        BOOL_COUNT = 12,
        FLOAT_COUNT = 0
    };

    bool mBoolData[ BOOL_COUNT ] = {false};
//    float mFloatData[ FLOAT_COUNT ] = { 0.0 };
    uint64_t mGroupID = 0;

    wchar_t const* mAdresses[ BOOL_COUNT + FLOAT_COUNT ] = {
        //bool
        L"CPU/DB33.s4_20ma",
        L"CPU/DB33.s0_20ma",
        L"CPU/DB33.s10v",
        L"CPU/DB33.s10ma",
        L"CPU/DB33.s15ma",
        L"CPU/DB33.s20ma",
        L"CPU/DB33.s40ma",
        L"CPU/DB33.channel_a",
        L"CPU/DB33.channel_b"
    };
};

}//namespace data
}//namespace cpu
